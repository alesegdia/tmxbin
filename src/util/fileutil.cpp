
#include "fileutil.h"


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

File::~File()
{
    if( m_file.is_open() )
    {
        m_file.close();
    }
}

void File::load(const char *path)
{
    if( m_file.is_open() )
    {
        close();
    }
    m_file.open(path, mode());
}

void File::close()
{
    m_file.close();
}

bool File::ok()
{
    return m_file.is_open();
}

std::fstream &File::file()
{
    return m_file;
}
