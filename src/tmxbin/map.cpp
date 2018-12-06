#include "map.h"


namespace tmxbin {


Map::Map(InputStream *istream, InputStreamBuilder *stream_builder, Allocator *allocator)
    : m_allocator(allocator)
{
    if( m_allocator == nullptr )
    {
        m_usingDefaultAllocator = true;
        m_allocator = new DefaultAllocator();
    }

    istream->readT<uint8_t>(reinterpret_cast<uint8_t*>(&m_orientation));
    istream->readT<uint8_t>(reinterpret_cast<uint8_t*>(&m_renderOrder));
    istream->readT<uint16_t>(&m_mapWidth);
    istream->readT<uint16_t>(&m_mapHeight);
    istream->readT<uint16_t>(&m_tileWidth);
    istream->readT<uint16_t>(&m_tileHeight);

    istream->readT<uint8_t>(&m_numTilesetEntries);
    m_tilesetEntries = m_allocator->allocT<TileSetEntry>(m_numTilesetEntries);
    for( uint8_t i = 0; i < m_numTilesetEntries; i++ )
    {
        TileSetEntry& tse = m_tilesetEntries[i];
        istream->readT<uint16_t>(&tse.firstGid);
        tse.source = istream->readStr(m_allocator);
        tse.tilesetObject = load_tileset( tse.source, stream_builder );
    }

    istream->readT<uint8_t>(&m_numLayers);
    m_tileMapLayers = m_allocator->allocT<TileMapLayer>(m_numLayers);
    for( uint8_t i = 0; i < m_numLayers; i++ )
    {
        TileMapLayer& tml = m_tileMapLayers[i];
        istream->readT<uint16_t>(&tml.width);
        istream->readT<uint16_t>(&tml.height);
        istream->readT<uint16_t>(&tml.id);
        tml.name = istream->readStr(m_allocator);

        uint16_t buffer_size = tml.width * tml.height;
        tml.data = static_cast<uint8_t*>(m_allocator->alloc(sizeof(uint8_t), buffer_size));
        istream->read(reinterpret_cast<char*>(tml.data), buffer_size);
    }
}

Map::~Map()
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

Orientation Map::orientation()
{
    return m_orientation;
}

RenderOrder Map::renderOrder()
{
    return m_renderOrder;
}

uint16_t Map::mapWidth()
{
    return m_mapWidth;
}

uint16_t Map::mapHeight()
{
    return m_mapHeight;
}

uint16_t Map::tileWidth()
{
    return m_tileWidth;
}

uint16_t Map::tileHeight()
{
    return m_tileHeight;
}

uint8_t Map::numTilesetEntries()
{
    return m_numTilesetEntries;
}

uint8_t Map::numLayers()
{
    return m_numLayers;
}

TileSetEntry *Map::getTilesetEntryByIndex(size_t index)
{
    if( index >= numTilesetEntries() )
    {
        return nullptr;
    }

    return &(m_tilesetEntries[index]);
}

TileMapLayer *Map::getTileMapLayerByIndex(size_t index)
{
    if( index >= numLayers() )
    {
        return nullptr;
    }

    return &(m_tileMapLayers[index]);
}


}

