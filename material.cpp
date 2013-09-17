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

void Material::WriteToStream(std::ofstream& stream)
{
    stream << this->_name << endl
           << "{" << endl;
    for (vector<string>::iterator i = this->_lines.begin(); i != this->_lines.end(); ++i)
        stream << (*i) << endl;
    stream << "}" << endl << endl;
}

int Material::LoadCollection(const std::string &filename, Material::Collection& collection)
{
    int startCount = int(collection.size());
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
    file.close();

    return (int(collection.size()) - startCount);
}

void Material::SaveCollection(const std::string& filename, Material::Collection& collection)
{
    ofstream output;
    output.open(filename.c_str(), ios::out);
    for (Collection::iterator i = collection.begin(); i != collection.end(); ++i)
        (*i).second->WriteToStream(output);
    output.close();
}
