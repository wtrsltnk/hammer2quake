#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class MapParser
{
public:
    /// Input is the full path to the input map file.
    MapParser(const std::vector<std::string>& args);
    virtual ~MapParser();

    /// Ouput is a path to the target file.
    virtual bool ParseMapFile() = 0;

    /// Ouput is a path to a folder.
    virtual bool ParseTextures() = 0;

    /// Print a friendly text about the input found through commanline parameters.
    virtual void PrintSummary() = 0;

    /// Print a friendly result text about the executed actions.
    virtual void PrintResults() = 0;

protected:
    /// Reads one line and tokenizes it
    std::string ReadNextLine();

    /// Returns true as long there is a new line
    bool HasNextLine();

protected:
    /// Input arguments from main()
    const std::vector<std::string> _args;

    /// Input MAP file
    std::string _input;

    /// The root folder where the input texture files can be found
    std::string _gameRoot;

private:
    std::ifstream _file;
};

#endif // MAPPARSER_H
