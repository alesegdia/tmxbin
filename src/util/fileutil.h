#pragma once

#include <fstream>
#include <cstring>


namespace tmxbin {

bool file_exists(const char* path);
std::string change_filestr_extension(const char* str, const char* newext);

}
