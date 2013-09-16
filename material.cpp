#include "material.h"
#include <iostream>
#include <fstream>

using namespace std;

Material::Material(const std::string& name)
    : _name(name)
{ }

Material::~Material()
{ }

void Material::AddLine(const std::string& line)
{
    this->_lines.push_back(line);
}

Material::Collection Material::LoadCollection(const std::string &filename)
{
    Material::Collection collection;

    ifstream file;
    file.open(filename.c_str(), std::ios::in);

    while (file.is_open() && file.good())
    {
        string name, open;
        getline(file, name);
        getline(file, open);
        if (open == string("{"))
        {
            Material* m = new Material(name);
            collection.insert(make_pair(name, m));
            while (file.is_open() && file.good())
            {
                string line;
                getline(file, line);
                if (line != string("}"))
                    m->_lines.push_back(line);
                else
                    break;
            }
        }
    }

    return collection;
}
