#pragma once

#include <istream>

namespace tmxbin {


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


}
