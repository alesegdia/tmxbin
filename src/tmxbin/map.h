#pragma once

#include <functional>

#include "orientation.h"
#include "renderorder.h"
#include "allocator.h"
#include "filestream.h"
#include "tileset.h"
#include "tilesetmanager.h"


namespace tmxbin {



struct TileSetEntry
{
    uint16_t firstGid;
    char* source;
    TileSet* tilesetObject;
};

struct TileMapLayer
{
    uint16_t width;
    uint16_t height;
    uint16_t id;
    char* name;
    uint8_t* data;
};

class Map
{
public:
    Map(
            InputStream* istream,
            InputStreamBuilder* stream_builder=nullptr,
            Allocator* allocator=nullptr);

    ~Map();

    Orientation orientation();
    RenderOrder renderOrder();

    uint16_t mapWidth();
    uint16_t mapHeight();
    uint16_t tileWidth();
    uint16_t tileHeight();

    uint8_t numTilesetEntries();
    TileSetEntry* getTilesetEntryByIndex(size_t index);

    uint8_t numLayers();
    TileMapLayer* getTileMapLayerByIndex(size_t index);
    TileMapLayer* getTileMapLayerByName(const char* name)
    {
        return nullptr;
    }

    void debug()
    {
        for( int tsindex = 0; tsindex < m_numTilesetEntries; tsindex++ ) {
            auto& tse = m_tilesetEntries[tsindex];
            std::cout << "Tileset " << tsindex << std::endl;
            tse.tilesetObject->debug();
        }
    }


private:
    Orientation m_orientation;
    RenderOrder m_renderOrder;
    uint16_t m_mapWidth;
    uint16_t m_mapHeight;
    uint16_t m_tileWidth;
    uint16_t m_tileHeight;

    uint8_t m_numTilesetEntries;
    TileSetEntry* m_tilesetEntries;

    uint8_t m_numLayers;
    TileMapLayer* m_tileMapLayers;

    Allocator* m_allocator;
    bool m_usingDefaultAllocator = false;


};



}
