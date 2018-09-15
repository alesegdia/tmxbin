#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "../../lib/pugixml/pugixml.hpp"

#include "../common/fileutil.h"
#include "../common/xmlutil.h"
#include "../common/enums.h"


int main( int argc, char** argv )
{
    if( argc != 2 )
    {
        std::cerr << "Need to provide input file" << std::endl;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(argv[1]);

    if( result == false )
    {
        std::cerr << "Couldn't open file " << argv[1] << " for reading" << std::endl;
        return -1;
    }

    std::string outfile_path = change_filestr_extension(argv[1], "tsb");

    OutputFile out;
    out.load(outfile_path.c_str());

    if( false == out.ok() )
    {
        std::cerr << "Couldn't open " << outfile_path << " for writing" << std::endl;
        return -1;
    }

    auto tileset_node = doc.select_node("tileset").node();

    pugi::xml_attribute attr_name = tileset_node.attribute("name");
    pugi::xml_attribute attr_tw = tileset_node.attribute("tilewidth");
    pugi::xml_attribute attr_th = tileset_node.attribute("tileheight");
    pugi::xml_attribute attr_tc = tileset_node.attribute("tilecount");
    pugi::xml_attribute attr_columns = tileset_node.attribute("columns");
    assert(attr_name != nullptr);
    assert(attr_tw != nullptr);
    assert(attr_th != nullptr);
    assert(attr_tc != nullptr);
    assert(attr_columns != nullptr);

    const char* name = attr_name.as_string();
    out.writeStr(name);

    uint16_t tw = uint16_t(attr_tw.as_uint());
    uint16_t th = uint16_t(attr_th.as_uint());
    uint16_t tc = uint16_t(attr_tc.as_uint());
    uint16_t columns = uint16_t(attr_columns.as_uint());

    out.write<uint16_t>(tw);
    out.write<uint16_t>(th);
    out.write<uint16_t>(tc);
    out.write<uint16_t>(columns);

    pugi::xml_node image_node = tileset_node.child("image");
    assert(image_node != nullptr);

    pugi::xml_attribute attr_source = image_node.attribute("source");
    assert(attr_source != nullptr);
    const char* source = attr_source.as_string();
    out.writeStr(source);

    pugi::xml_attribute attr_iw = image_node.attribute("width");
    pugi::xml_attribute attr_ih = image_node.attribute("height");
    assert(attr_iw != nullptr);
    assert(attr_ih != nullptr);
    uint16_t iw, ih;
    iw = uint16_t(attr_iw.as_uint());
    ih = uint16_t(attr_ih.as_uint());
    out.write<uint16_t>(iw);
    out.write<uint16_t>(ih);

    auto tile_nodes = get_children(tileset_node, "tile");

    out.write<uint16_t>(uint16_t(tile_nodes.size()));

    for( auto& tile_node : tile_nodes )
    {
        pugi::xml_attribute attr_id = tile_node.attribute("id");
        assert(attr_id != nullptr);
        uint16_t id = uint16_t(attr_id.as_uint());

        pugi::xml_node props_root_node = tile_node.child("properties");
        auto prop_nodes = get_children(props_root_node, "property");

        out.write<uint16_t>(uint16_t(prop_nodes.size()));

        for( auto& prop_node : prop_nodes )
        {
            pugi::xml_attribute attr_name = prop_node.attribute("name");
            pugi::xml_attribute attr_type = prop_node.attribute("type");
            pugi::xml_attribute attr_value = prop_node.attribute("value");
            assert(attr_name != nullptr);
            assert(attr_type != nullptr);
            assert(attr_value != nullptr);
            const char* prop_name = attr_name.as_string();
            const char* prop_type = attr_type.as_string();
            const char* prop_value = attr_value.as_string();
            out.writeStr(prop_name);
            out.writeStr(prop_type);
            out.writeStr(prop_value);
        }
    }

    return 0;
}
