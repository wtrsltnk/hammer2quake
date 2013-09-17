#include "mapparser.h"

MapParser::MapParser(const std::vector<std::string>& args)
    : _args(args)
{
    for (int i = 0; i < int(args.size()); i++)
    {
        std::string arg = args[i];
        if (arg == std::string("--input") && i + 1 < int(args.size()))
            this->_input = args[i + 1];
        else if (arg == std::string("--game-root") && i + 1 < int(args.size()))
            this->_gameRoot = args[i + 1];
    }
    this->_file.open(this->_input.c_str(), std::ios::in);
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
