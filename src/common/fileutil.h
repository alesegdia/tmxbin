#pragma once

#include <fstream>
#include <cstring>

class File
{
public:
    virtual ~File();

    void load( const char* path );
    void close();
    bool ok();

protected:
    virtual std::ios::openmode mode() = 0 ;
    std::fstream& file();

private:
    std::fstream m_file;

};

class OutputFile : public File
{
public:
    template <typename T>
    void write(T value)
    {
        file().write(reinterpret_cast<char*>(&value), sizeof(T));
    }

    void writeStr(const char* str)
    {
        uint16_t sz = uint16_t(strlen(str));
        write<uint16_t>(uint16_t(sz));
        file().write(str, sz);
    }

private:
    std::ios::openmode mode() override
    {
        return std::ios::binary |
               std::ios::trunc |
               std::ios::out;
    }

};


bool file_exists(const char* path);

std::string change_filestr_extension(const char* str, const char* newext);
