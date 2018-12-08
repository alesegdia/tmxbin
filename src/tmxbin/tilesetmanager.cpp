#include <vector>

#include "tilesetmanager.h"

namespace tmxbin {


static std::vector<TileSet*> tilesets;
static Allocator* tileset_allocator = nullptr;
static bool using_default_allocator = false;


// Utility to create your own input streams *********************************************
static InputStream *default_create_tileset_input_stream_cb(const char *src)
{
    StdInputFile* file = new StdInputFile;
    file->open(src);
    return file;
}

static void default_destroy_tileset_input_stream_cb(InputStream *is)
{
    delete is;
}

static InputStreamBuilder default_input_stream_builder({
    default_create_tileset_input_stream_cb,
    default_destroy_tileset_input_stream_cb
});
// **************************************************************************************

static Allocator* get_tileset_allocator()
{
    if( tileset_allocator == nullptr )
    {
        tileset_allocator = new DefaultAllocator();
        using_default_allocator = true;
    }
    return tileset_allocator;
}

static TileSet* get_tileset_by_path( const char* path )
{
    // try to find the tileset, otherwise return nullptr
    return nullptr;
}

TileSet* load_tileset(const char* path, InputStreamBuilder* cisb)
{
    if( cisb == nullptr )
    {
        cisb = &default_input_stream_builder;
    }

    if( cisb->create == nullptr || cisb->destroy == nullptr )
    {
        return nullptr;
    }

    TileSet* ts = get_tileset_by_path(path);
    if( ts == nullptr )
    {
        InputStream* is = cisb->create(path);
        Allocator* allocator = get_tileset_allocator();
        ts = allocator->construct<TileSet>(is, allocator);
        cisb->destroy(is);
        tilesets.push_back(ts);
    }

    return ts;
}

void destroy_tilesets()
{
    for( auto t : tilesets )
    {
        tileset_allocator->dealloc(t);
    }

    if( using_default_allocator )
    {
        delete tileset_allocator;
    }
}

void set_custom_tileset_allocator(Allocator* allocator, uint8_t &status)
{
    if( using_default_allocator )
    {
        // already using default allocator
        status = 1;
    }
    else if( tileset_allocator != nullptr )
    {
        // allocator already set;
        status = 2;
    }
    else
    {
        tileset_allocator = allocator;
        status = 0;
    }
}


}
