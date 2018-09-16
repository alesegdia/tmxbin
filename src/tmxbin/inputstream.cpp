#include "inputstream.h"


namespace tmxbin {


InputStream::~InputStream()
{

}

StdInputStream::StdInputStream(std::istream &stream)
    : m_istream(stream)
{

}

StdInputStream::~StdInputStream()
{

}

void StdInputStream::read(char *out, size_t size)
{
    m_istream.read(out, std::streamsize(size));
}


}
