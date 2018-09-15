#include "xmlutil.h"

std::vector<pugi::xml_node> get_children(const pugi::xml_node& node, const char* childname)
{
    std::vector<pugi::xml_node> outvec;
    for( pugi::xml_node child : node.children(childname) )
    {
        outvec.push_back(child);
    }
    return outvec;
}

