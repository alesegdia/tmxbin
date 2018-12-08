#pragma once

#include <unordered_map>

#include "filestream.h"


namespace tmxbin {


class TileProperty
{
public:


private:
    std::string m_type;
    std::string m_value;

};

struct Tile {
    std::unordered_map<std::string, std::string> properties;
};

class TileSet
{
public:
    TileSet( InputStream* is, Allocator* allocator )
        : m_allocator(allocator)
    {
        if( m_allocator == nullptr )
        {
            m_usingDefaultAllocator = true;
            m_allocator = new DefaultAllocator();
        }

        m_name = is->readStr(m_allocator);
        is->readT<uint16_t>(&m_tileWidth);
        is->readT<uint16_t>(&m_tileHeight);
        is->readT<uint16_t>(&m_tileCount);
        is->readT<uint16_t>(&m_columns);

        m_path = is->readStr(m_allocator);
        is->readT<uint16_t>(&m_imageWidth);
        is->readT<uint16_t>(&m_imageHeight);
    }

private:

    bool m_usingDefaultAllocator = false;
    Allocator* m_allocator;

    // General info
    char* m_name;
    uint16_t m_tileWidth;
    uint16_t m_tileHeight;
    uint16_t m_tileCount;
    uint16_t m_columns;

    // Image
    char* m_path;
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;

    // Tiles
    uint8_t m_numTiles;
    Tile* m_tiles;

};


}
