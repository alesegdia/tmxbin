#pragma once

#include <vector>

#include "../../lib/pugixml/pugixml.hpp"

std::vector<pugi::xml_node> get_children(const pugi::xml_node& node, const char* childname);
