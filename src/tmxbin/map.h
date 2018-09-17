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
            Allocator* allocator=nullptr)
        : m_allocator(allocator)
    {
        if( m_allocator == nullptr )
        {
            m_usingDefaultAllocator = true;
            m_allocator = new DefaultAllocator();
        }

        istream->readT<uint8_t>(reinterpret_cast<uint8_t*>(&m_orientation));
        istream->readT<uint8_t>(reinterpret_cast<uint8_t*>(&m_renderOrder));
        istream->readT<uint16_t>(reinterpret_cast<uint16_t*>(m_mapWidth));
        istream->readT<uint16_t>(reinterpret_cast<uint16_t*>(m_mapHeight));
        istream->readT<uint16_t>(reinterpret_cast<uint16_t*>(m_tileWidth));
        istream->readT<uint16_t>(reinterpret_cast<uint16_t*>(m_tileHeight));

        istream->readT<uint8_t>(&m_numTilesetEntries);
        m_tilesetEntries = allocator->alloc<TileSetEntry>(m_numTilesetEntries);
        for( uint8_t i = 0; i < m_numTilesetEntries; i++ )
        {
            TileSetEntry& tse = m_tilesetEntries[i];
            istream->readT<uint16_t>(&tse.firstGid);
            tse.source = istream->readStr(m_allocator);
            tse.tilesetObject = load_tileset( tse.source, stream_builder );
        }

        istream->readT<uint8_t>(&m_numLayers);
        m_tileMapLayers = allocator->alloc<TileMapLayer>(m_numLayers);
        for( uint8_t i = 0; i < m_numLayers; i++ )
        {
            TileMapLayer tml = m_tileMapLayers[i];
            istream->readT<uint16_t>(&tml.width);
            istream->readT<uint16_t>(&tml.height);
            istream->readT<uint16_t>(&tml.id);
            tml.name = istream->readStr(m_allocator);

            uint16_t buffer_size = tml.width * tml.height;
            m_allocator->alloc(sizeof(uint8_t), buffer_size);
            istream->read(reinterpret_cast<char*>(tml.data), buffer_size);
        }
    }

    ~Map()
    {
        // clear tileset entries
        for( uint8_t i = 0; i < m_numTilesetEntries; i++ )
        {
            m_allocator->dealloc(m_tilesetEntries[i].source);
        }
        m_allocator->dealloc(m_tilesetEntries);

        // clear tile map layers
        for( uint8_t i = 0 ; i < m_numLayers; i++ )
        {
            m_allocator->dealloc(m_tileMapLayers[i].name);
            m_allocator->dealloc(m_tileMapLayers[i].data);
        }
        m_allocator->dealloc(m_tileMapLayers);

        if( m_usingDefaultAllocator )
        {
            delete m_allocator;
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
