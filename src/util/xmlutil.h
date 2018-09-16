#pragma once

#include <vector>

#include "../pugixml/pugixml.hpp"

std::vector<pugi::xml_node> get_children(const pugi::xml_node& node, const char* childname);
