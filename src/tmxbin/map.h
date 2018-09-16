#pragma once

#include "orientation.h"
#include "renderorder.h"
#include "inputstream.h"
#include "allocator.h"


namespace tmxbin {


struct TileSetEntry
{
    uint16_t firstGid;
    char* source;
};

class Map
{
public:
    Map(InputStream* istream, Allocator* allocator)
        : m_allocator(allocator)
    {
        istream->read<uint8_t>(reinterpret_cast<uint8_t*>(&m_orientation));
        istream->read<uint8_t>(reinterpret_cast<uint8_t*>(&m_renderOrder));
        istream->read<uint16_t>(reinterpret_cast<uint16_t*>(m_mapWidth));
        istream->read<uint16_t>(reinterpret_cast<uint16_t*>(m_mapHeight));
        istream->read<uint16_t>(reinterpret_cast<uint16_t*>(m_tileWidth));
        istream->read<uint16_t>(reinterpret_cast<uint16_t*>(m_tileHeight));

        istream->read<uint8_t>(&m_numTilesetEntries);

        m_tilesetEntries = allocator->alloc<TileSetEntry>(m_numTilesetEntries);

        for( uint8_t i = 0; i < m_numTilesetEntries; i++ )
        {

        }
    }

    ~Map()
    {
        for( uint8_t i = 0; i < m_numTilesetEntries; i++ )
        {
            m_allocator->dealloc(m_tilesetEntries[i].source);
        }
        m_allocator->dealloc(m_tilesetEntries);
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
    Allocator* m_allocator;

};



}
