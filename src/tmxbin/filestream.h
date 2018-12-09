#pragma once

#include <istream>
#include <fstream>

#include "allocator.h"

namespace tmxbin {


class OutputStream
{
public:
    virtual ~OutputStream();
    virtual void write( const char*, size_t num_elements ) = 0 ;
    void writeStr(const char* str);

    template <typename T>
    void writeT( T value)
    {
        write(reinterpret_cast<char*>(&value), sizeof(T));
    }

};

class InputStream
{
public:
    virtual ~InputStream();
    virtual void read( char* out_buffer, size_t num_elements ) = 0 ;
    char* readStr(tmxbin::Allocator* allocator);

    template <typename T>
    void readT(T* out)
    {
        read(reinterpret_cast<char*>(out), sizeof(T));
    }
};


class File
{
public:
    virtual ~File();
    virtual void close() = 0 ;
    virtual bool ok() = 0 ;

	const std::string& path() {
		return m_path;
	}

	void openPath(const char* path)
	{
		m_path = path;
		open(path);
	}

private:
	virtual void open(const char* path) = 0 ;

	std::string m_path;

};

class StdFile : public File
{
public:
    virtual ~StdFile() override;
    void close() override;
    bool ok() override;
protected:
    std::fstream m_file;
};

class StdInputFile : public StdFile, public InputStream
{
public:
    virtual ~StdInputFile() override;
    void read(char *out_buffer, size_t num_elements) override;

private:
	void open(const char* path) override;
};

class StdOutputFile : public StdFile, public OutputStream
{
public:
    virtual ~StdOutputFile() override;
    void write(const char* buffer, size_t num_elements) override;

private:
	void open(const char* path) override;
};



}
