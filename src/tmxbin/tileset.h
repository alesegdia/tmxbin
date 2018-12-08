#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>

#include "filestream.h"


namespace tmxbin {


struct TileProperty
{
    std::string name;
    std::string type;
    std::string value;
};

struct Tile {
    uint16_t id;
    std::unordered_map<std::string, TileProperty> properties;
    bool hasProperty(std::string prop)
    {
        return properties.count(prop);
    }
};

class TileSet
{
public:
    TileSet( InputStream* is, Allocator* allocator );

    void debug();

private:

    bool m_usingDefaultAllocator = false;
    Allocator* m_allocator;

    // General info
    std::string m_name;
    uint16_t m_tileWidth;
    uint16_t m_tileHeight;
    uint16_t m_tileCount;
    uint16_t m_columns;

    // Image
    std::string m_path;
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;

    // Tiles
    std::vector<Tile> m_tiles;

};


}
