#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <string>
#include <vector>
#include <stdio.h>

class MapParser
{
public:
    /// Input is the full path to the input map file.
    MapParser(const std::string& input);
    virtual ~MapParser();

    /// Ouput is a path to the target file.
    virtual bool ParseMapFile(const std::string& output) = 0;

    /// Ouput is a path to a folder.
    virtual bool ParseTextures(const std::string& output) = 0;

    /// Sets the gameroot which might be helpfull parsing the map/textures
    void SetGameRoot(const std::string& gameRoot) { this->_gameRoot = gameRoot; }

protected:
    /// Reads one line and tokenizes it
    std::vector<std::string> ReadNextLine();

    /// Returns true as long there is a new line
    bool HasNextLine();

protected:
    const std::string& _input;
    std::string _gameRoot;

private:
    FILE* _file;
};

#endif // MAPPARSER_H
