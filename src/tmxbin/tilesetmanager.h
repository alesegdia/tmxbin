#pragma once

#include <functional>


#include "tileset.h"
#include "allocator.h"


namespace tmxbin {


struct InputStreamBuilder
{
    std::function<InputStream*(const char*)> create;
    std::function<void(InputStream*)> destroy;
};

void set_custom_tileset_allocator(Allocator* allocator, uint8_t& status);
TileSet* load_tileset(const char* path, InputStreamBuilder* cisb = nullptr);
void destroy_tilesets();
static TileSet* get_tileset_by_path( const char* path );


}
