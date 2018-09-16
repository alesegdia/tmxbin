
#include "fileutil.h"


namespace tmxbin {


bool file_exists(const char *path)
{
    std::ifstream f(path);
    bool exists = f.good();
    if( f.is_open() )
    {
        f.close();
    }
    return exists;
}

std::string change_filestr_extension(const char *str, const char *newext)
{
    std::string s(str);
    return s.substr(0, s.find(".")) + "." + newext;
}


}
