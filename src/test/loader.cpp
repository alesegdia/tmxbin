#include "../../module/xstf/include/xstf.h"

#include "../tmxbin/tmxbin.h"

#include <fstream>

int main( int argc, char** argv )
{
    ((void)argc);
    ((void)argv);

    XSTF_TESTING("TMXBin - Tiled Map Binary Format");
    XSTF_SET_VERBOSITY_LEVEL(XSTF_VL_FULL);

    {
        XSTF_TEST("Loading");
        tmxbin::StdInputFile file;
        file.openPath("testdata/simplemap.tmb");
        tmxbin::Map map(&file);
        XSTF_ASSERT(map.numLayers() == 2, "Incorrect number of layers");
    }

    tmxbin::destroy_tilesets();

    return 0;
}
