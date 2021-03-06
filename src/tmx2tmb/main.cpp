#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string.h>
#include <stdio.h>

#include "../pugixml/pugixml.hpp"

#include "../util/fileutil.h"
#include "../util/xmlutil.h"
#include "../tmxbin/tmxbin.h"

tmxbin::Orientation parse_orientation( const pugi::xml_node& mapnode )
{
    auto attr_res = mapnode.attribute("orientation");
    assert(attr_res != nullptr);
    auto str = attr_res.as_string();

    if( strcmp(str, "orthogonal") == 0 )
    {
        return tmxbin::Orientation::Orthogonal;
    }
    else
    {
        return tmxbin::Orientation::Isometric;
    }
}

tmxbin::RenderOrder parse_renderorder( const pugi::xml_node& mapnode )
{
    pugi::xml_attribute attr_res = mapnode.attribute("renderorder");
    assert(attr_res != nullptr);
    auto str = attr_res.as_string();
    if( strcmp(str, "right-down") == 0 ) return tmxbin::RenderOrder::RightDown;
    if( strcmp(str, "right-up") == 0 ) return tmxbin::RenderOrder::RightUp;
    if( strcmp(str, "left-down") == 0 ) return tmxbin::RenderOrder::LeftDown;
    return tmxbin::RenderOrder::LeftUp;
}

void parse_mapsize( const pugi::xml_node& mapnode, uint16_t* outw, uint16_t* outh )
{
    pugi::xml_attribute attr_mapw = mapnode.attribute("width");
    pugi::xml_attribute attr_maph = mapnode.attribute("height");
    assert(attr_mapw != nullptr);
    assert(attr_maph != nullptr);
    *outw = uint16_t(attr_mapw.as_int());
    *outh = uint16_t(attr_maph.as_int());
}

void parse_tilesize( const pugi::xml_node& mapnode, uint16_t* outw, uint16_t* outh )
{
    pugi::xml_attribute attr_tilew = mapnode.attribute("tilewidth");
    pugi::xml_attribute attr_tileh = mapnode.attribute("tileheight");
    assert(attr_tilew != nullptr);
    assert(attr_tileh != nullptr);
    *outw = uint16_t(attr_tilew.as_int());
    *outh = uint16_t(attr_tileh.as_int());
}

void write_tileset( tmxbin::StdOutputFile& of, pugi::xml_node tileset )
{
    pugi::xml_attribute attr_firstgid = tileset.attribute("firstgid");
    pugi::xml_attribute attr_source = tileset.attribute("source");

    if( attr_source == nullptr )
    {
        std::cerr << "The tileset must be an external .tsx file" << std::endl;
        exit(-1);
    }

    of.writeT<uint16_t>(uint16_t(attr_firstgid.as_int()) );
    std::string source_file = std::string(attr_source.as_string());
    auto new_str = tmxbin::get_folder(of.path()) + "/" + tmxbin::change_filestr_extension(source_file.c_str(), "tsb");

    std::cout << "Writing tileset: " << new_str << std::endl;

    of.writeStr(new_str.c_str());
}

void write_layer( tmxbin::StdOutputFile& of, pugi::xml_node layer )
{
    // process layer metadata
    pugi::xml_attribute attr_lw = layer.attribute("width");
    pugi::xml_attribute attr_lh = layer.attribute("height");
    pugi::xml_attribute attr_id = layer.attribute("id");
    pugi::xml_attribute attr_name = layer.attribute("name");

    assert(attr_lw != nullptr);
    assert(attr_lh != nullptr);
    assert(attr_id != nullptr);
    assert(attr_name != nullptr);

    of.writeT<uint16_t>(uint16_t(attr_lw.as_int()));
    of.writeT<uint16_t>(uint16_t(attr_lh.as_int()));
    of.writeT<uint16_t>(uint16_t(attr_id.as_int()));
    of.writeStr(attr_name.as_string());

    // now process the layer positioning data
    pugi::xml_node data = layer.child("data");
    pugi::xml_attribute attr_encoding = data.attribute("encoding");
    assert(attr_encoding != nullptr);
    const char* encoding = attr_encoding.as_string();
    assert(strcmp(encoding, "csv") == 0 && "Encoding must be CSV");

    const char* layer_data = data.text().as_string();
    std::cout << layer_data << std::endl;
    const char* ptr = layer_data;
    uint16_t accumulator = 0;
    bool number_found = false;
    int32_t len = int32_t(strlen(layer_data));

    int elemens_count = 0;
    while( *ptr != '\0' )
    {
        bool char_is_number = *ptr >= '0' && *ptr <= '9';
        bool is_last_element = (ptr - layer_data) == (len - 1);

        if( char_is_number )
        {
            uint8_t num = uint8_t(*ptr - '0');
            number_found = true;
            accumulator *= 10;
            accumulator += num;
        }

        if( false == char_is_number || is_last_element )
        {
            if( number_found )
            {
                assert(accumulator <= 255 && "Only supporting 1 bitemap (.bandcamp.com)");
                of.writeT<uint8_t>(uint8_t(accumulator));
                elemens_count++;
            }
            number_found = false;
            accumulator = 0;
        }
        ptr++;
    }

    printf("%d\n", elemens_count);
}

/**
 * 0a0b0c1d1e2f1g1h0i1j2k
 * ├── a
 * ├── b
 * ├── c
 * │   ├── d
 * │   ├── e
 * │   │   └── f
 * │   ├── g
 * │   └── h
 * └── i
 *     └── j
 *         └── k
 */
void write_group( tmxbin::StdOutputFile& out, pugi::xml_node node, uint8_t depth = 0 )
{
    auto tilesets = tmxbin::get_children(node, "group");
    out.writeT<uint8_t>( uint8_t(tilesets.size()) );
    for( auto& tileset : tilesets )
    {
        write_tileset(out, tileset);
    }
}

int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        std::cerr << "Need to provide input file" << std::endl;
        return -1;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(argv[1]);

    if( result == false )
    {
        std::cerr << "Couldn't open file " << argv[1] << " for reading" << std::endl;
        return -1;
    }

    std::string outfile_path = tmxbin::change_filestr_extension(argv[1], "tmb");

    tmxbin::StdOutputFile out;
    out.openPath(outfile_path.c_str());

    if( false == out.ok() )
    {
        std::cerr << "Couldn't open " << outfile_path << " for writing" << std::endl;
        return -1;
    }

    auto map_node = doc.select_node("map").node();

    out.writeT<uint8_t>( uint8_t(parse_orientation(map_node)) );
    out.writeT<uint8_t>( uint8_t(parse_renderorder(map_node)) );

    uint16_t mw, mh, tw, th;
    parse_mapsize(map_node, &mw, &mh);
    parse_tilesize(map_node, &tw, &th);

    out.writeT<uint16_t>( uint16_t(mw) );
    out.writeT<uint16_t>( uint16_t(mh) );
    out.writeT<uint16_t>( uint16_t(tw) );
    out.writeT<uint16_t>( uint16_t(th) );

    auto tilesets = tmxbin::get_children(map_node, "tileset");
    out.writeT<uint8_t>( uint8_t(tilesets.size()) );
    for( auto& tileset : tilesets )
    {
        write_tileset(out, tileset);
    }

    auto layers = tmxbin::get_children(map_node, "layer");
    out.writeT<uint8_t>( uint8_t(layers.size()) );
    for( auto& layer : layers )
    {
        write_layer(out, layer);
    }

    out.close();

    return 0;
}
