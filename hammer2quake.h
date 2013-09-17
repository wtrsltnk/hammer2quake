#ifndef HAMMER2QUAKE_H
#define HAMMER2QUAKE_H

#include "mapparser.h"
#include "material.h"
#include <set>

class Hammer2Quake : public MapParser
{
public:
    Hammer2Quake(const std::vector<std::string>& args);
    virtual ~Hammer2Quake();

    virtual bool ParseMapFile();
    virtual bool ParseTextures();

    virtual void PrintSummary() { }
    virtual void PrintResults() { }

private:
    std::string _outputMap;
    std::string _outputRoot;
    std::string _shaderScript;
    std::string _wads;
    std::set<std::string> _textures;
    Material::Collection _materials;
};

#endif // HAMMER2QUAKE_H
