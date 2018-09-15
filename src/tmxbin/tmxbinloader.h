#pragma once

#include <istream>

#include "../common/enums.h"


namespace tmxbin {


enum class Orientation : std::uint8_t
{
    Orthogonal = 0,
    Isometric
};

enum class RenderOrder : std::uint8_t
{
    RightUp = 0,
    RightDown,
    LeftUp,
    LeftDown
};

class InputStream
{
public:
    virtual ~InputStream() = 0 ;

    template <typename T>
    void read(T* out)
    {
        read(reinterpret_cast<char*>(out), sizeof(T));
    }

protected:
    virtual void read(char* out, size_t size) = 0 ;

};

class StdInputStream : public InputStream
{
public:
    StdInputStream(std::istream& stream);

    virtual ~StdInputStream();

private:
    virtual void read(char* out, size_t size) final;

    std::istream& m_istream;

};


struct TileSetEntry
{
    uint16_t firstGid;
    char* source;
};


class Allocator
{
public:
    virtual ~Allocator();

    template <typename T>
    T* alloc(size_t num_elements)
    {
        return static_cast<T*>(alloc(sizeof(T), num_elements));
    }

    template <typename T>
    void dealloc(T* memory)
    {
        free(memory);
    }

protected:
    virtual void* alloc(size_t element_size, size_t element_count) = 0 ;
    virtual void dealloc(void* memory) = 0 ;

};

class DefaultAllocator : public Allocator
{
private:
    virtual void* alloc(size_t element_size, size_t element_count) final
    {
        return malloc(element_size * element_count);
    }

    virtual void dealloc(void* memory) final
    {
        free(memory);
    }
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

class TileSet
{
public:
    TileSet(InputStream* istream)
    {

    }
};


}
