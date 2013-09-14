#include "mapparser.h"

MapParser::MapParser(const std::string& input)
    : _input(input)
{
    this->_file = fopen(input.c_str(), "r");
}

MapParser::~MapParser()
{
    if (this->_file != 0)
        fclose(this->_file);
    this->_file = 0;
}

std::vector<std::string> MapParser::ReadNextLine()
{
    std::vector<std::string> result;

    return result;
}

bool MapParser::HasNextLine()
{
    return true;
}
