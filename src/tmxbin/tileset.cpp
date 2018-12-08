#include "tileset.h"

namespace tmxbin {

TileSet::TileSet(InputStream *is, Allocator *allocator)
    : m_allocator(allocator)
{
    if( m_allocator == nullptr )
    {
        m_usingDefaultAllocator = true;
        m_allocator = new DefaultAllocator();
    }

    char* name = is->readStr(m_allocator);
    m_name = std::string(name);
    std::cout << name << std::endl;
    std::cout << m_name << std::endl;

    m_allocator->dealloc(name);

    is->readT<uint16_t>(&m_tileWidth);
    is->readT<uint16_t>(&m_tileHeight);
    is->readT<uint16_t>(&m_tileCount);
    is->readT<uint16_t>(&m_columns);

    auto path = is->readStr(m_allocator);
    m_path = std::string(path);
    m_allocator->dealloc(path);

    is->readT<uint16_t>(&m_imageWidth);
    is->readT<uint16_t>(&m_imageHeight);

    uint16_t numTiles;
    is->readT<uint16_t>(&numTiles);
    std::cout << "numtiles: " << numTiles << std::endl;
    for( int i = 0; i < numTiles; i++ ) {
        Tile tile;
        is->readT<uint16_t>(&tile.id);

        uint16_t numProps;
        is->readT<uint16_t>(&numProps);

        for( int propIndex = 0; propIndex < numProps; propIndex++ ) {
            char *name, *type, *value;
            name = is->readStr(m_allocator);
            type = is->readStr(m_allocator);
            value = is->readStr(m_allocator);

            TileProperty property{name, type, value};

            m_allocator->dealloc(name);
            m_allocator->dealloc(type);
            m_allocator->dealloc(value);

            tile.properties[property.name] = property;
        }
        m_tiles.push_back(tile);
    }
}

void TileSet::debug()
{
    std::cout << "Name: " << m_name << std::endl;
    for( const auto& tile : m_tiles ) {
        std::cout << "\tID: " << tile.id << std::endl;
        std::cout << "\trops"<< std::endl;
        for( const auto& pair : tile.properties ) {
            std::cout << "\t\t" << pair.first << ": " << pair.second.value << "[" << pair.second.type << "]" << std::endl;
        }
    }
}


}
