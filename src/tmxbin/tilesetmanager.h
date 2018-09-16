#pragma once

#include "tileset.h"
#include "inputstream.h"


namespace tmxbin {


class TileSetManager
{
public:
    uint8_t getIndexForTilesetPath(const char* path)
    {

    }

    uint8_t loadTileSet(const char* id, InputStream& is)
    {

    }

private:
    TileSet* m_tileSets;
};


}
