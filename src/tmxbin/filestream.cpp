#include "filestream.h"

#include <cstring>

namespace tmxbin {


OutputStream::~OutputStream()
{

}

void OutputStream::writeStr(const char *str)
{
    uint16_t sz = uint16_t(strlen(str));
    writeT<uint16_t>(uint16_t(sz));
    write(str, sz);
}

InputStream::~InputStream()
{

}

char *InputStream::readStr(Allocator *allocator)
{
    uint16_t size;
    readT<uint16_t>(&size);
    char* buffer = allocator->alloc<char>(size);
    read( buffer, size );
    return buffer;
}

File::~File()
{

}

StdFile::~StdFile()
{

}

void StdFile::close()
{
    m_file.close();
}

bool StdFile::ok()
{
    return m_file.is_open() && m_file.good();
}

StdInputFile::~StdInputFile()
{

}

void StdInputFile::open(const char *path)
{
    auto flags = std::ios::binary |
            std::ios::in;
    m_file.open(path, flags);
}

void StdInputFile::read(char *out_buffer, size_t num_elements)
{
    m_file.read(out_buffer, std::streamsize(num_elements));
}

StdOutputFile::~StdOutputFile()
{

}

void StdOutputFile::open(const char *path)
{
    auto flags = std::ios::binary |
            std::ios::out |
            std::ios::trunc;
    m_file.open(path, flags);
}

void StdOutputFile::write(const char *buffer, size_t num_elements)
{
    m_file.write(buffer, std::streamsize(num_elements));
}



}
