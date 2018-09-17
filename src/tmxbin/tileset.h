#pragma once

#include "filestream.h"


namespace tmxbin {


class TileSet
{
public:
    TileSet( InputStream& is, Allocator* allocator )
        : m_allocator(allocator)
    {

    }

private:
    Allocator* m_allocator;

};


}
