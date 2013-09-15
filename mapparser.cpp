#include "mapparser.h"

MapParser::MapParser(const std::string& input)
    : _input(input)
{
    this->_file.open(input.c_str(), std::ios::in);
}

MapParser::~MapParser()
{
    this->_file.close();
}

std::string MapParser::ReadNextLine()
{
    std::string result;

    if (this->_file.is_open() && this->_file.good())
        std::getline(this->_file, result);

    return result;
}

bool MapParser::HasNextLine()
{
    return (this->_file.is_open() && this->_file.good());
}
