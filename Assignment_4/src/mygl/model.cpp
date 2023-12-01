#include "model.h"

#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace detail
{

void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;

    while( (start = str.find_first_not_of(delim, end)) != std::string::npos )
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

struct Index
{
    enum eType
    {
        V = 1,
        VN = 2,
        VT = 4,

        V_VN = V | VN,
        V_VT_VN = V | VN | VT
    };

    eType type = V;
    unsigned int v = 0;
    unsigned int vt = 0;
    unsigned int vn = 0;

    friend std::stringstream& operator >>(std::stringstream& in, Index& index)
    {
        std::string data;
        in >> data;

        std::vector<std::string> tokens;
        tokenize(data, '/', tokens);

        if(tokens.empty())
        {
            return in;
        }

        index.v = std::stoi( tokens[0] );

        if(tokens.size() == 2)
        {
            index.vn = std::stoi( tokens[1] );
            index.type = V_VN;
        }
        else if(tokens.size() == 3)
        {
            index.vt = std::stoi( tokens[1] );
            index.vn = std::stoi( tokens[2] );
            index.type = V_VT_VN;
        }

        return in;
    }
};

}

std::map<std::string, Material> materialLoad(const std::string &filepath)
{
    std::ifstream materialFile(filepath);
    if(!materialFile.is_open())
    {
        throw std::runtime_error("[Model] Couldn't open OBJ file at " + filepath);
    }

    std::map<std::string, Material> materials;
    Material* current = nullptr;

    /* consume material commands */
    std::string line;
    while(std::getline(materialFile, line))
    {
        std::stringstream ss(line);

        /* command code */
        std::string code;
        ss >> code;

        /* create new material */
        if(code == "newmtl")
        {
            Material material;
            ss >> material.name;

            materials[material.name] = material;
            current = &materials[material.name];
        }
        /* shininess parameter */
        else if(code == "Ns" && current)
        {
            float ns = 1.0f;
            ss >> ns;

            current->shininess = ns;
        }
        /* ambient color */
        else if(code == "Ka" && current)
        {
            ss >> current->ambient.x >> current->ambient.y >> current->ambient.z;
        }
        /* diffuse color */
        else if(code == "Kd" && current)
        {
            ss >> current->diffuse.x >> current->diffuse.y >> current->diffuse.z;
        }
        /* specular color */
        else if(code == "Ks" && current)
        {
            ss >> current->specular.x >> current->specular.y >> current->specular.z;
        }
        /* emission color */
        else if(code == "Ke" && current)
        {
            ss >> current->emission.x >> current->emission.y >> current->emission.z;
        }
    }

    return materials;
}

std::vector<Model> modelLoad(const std::string &filepath)
{
    std::ifstream objFile(filepath);
    if(!objFile.is_open())
    {
        throw std::runtime_error("[Model] Couldn't open OBJ file at " + filepath);
    }

    /* container for GL related stuff */
    std::vector<Model> models;
    std::vector<Vertex> glVertices;
    std::vector<unsigned int> glIndices;

    /* container for OBJ related stuff */
    std::map<std::string, Material> materials;
    std::vector<Vector3D> vertices;
    std::vector<Vector3D> normals;
    std::vector<Vector2D> uvs;


    /* consume commonds from obj file */
    std::string line;
    while(std::getline(objFile, line))
    {
        std::stringstream ss(line);

        /* command code */
        std::string code;
        ss >> code;

        if(code == "")
        {
            continue;
        }
        /* create new object */
        else if(code == "o")
        {
            if(!models.empty())
            {
                Model& model = models.back();
                model.mesh = meshCreate(glVertices, glIndices);

                if(!model.material.empty())
                {
                    auto& material = model.material.back();
                    material.indexCount = glVertices.size() - material.indexOffset;
                }

                glVertices.clear();
                glIndices.clear();
            }

            Model& model = models.emplace_back();
            ss >> model.name;
        }
        /* vertex postion */
        else if(code == "v")
        {
            auto& v = vertices.emplace_back();
            ss >> v.x >> v.y >> v.z;
        }
        /* vertex texture coordinates */
        else if(code == "vt")
        {
            auto& vt = uvs.emplace_back();
            ss >> vt.x >> vt.y;
        }
        /* vertex normal */
        else if(code == "vn")
        {
            auto& vn = normals.emplace_back();
            ss >> vn.x >> vn.y >> vn.z;
        }
        /* face definition (currently only triangles) */
        else if(code == "f")
        {
            detail::Index _idx[3];
            ss >> _idx[0] >> _idx[1] >> _idx[2];

            for(int i = 0; i < 3; i++)
            {
                glIndices.emplace_back(glVertices.size());

                Vertex& vertex = glVertices.emplace_back();
                vertex.pos = vertices[_idx[i].v - 1];

                if(_idx[i].type == detail::Index::V_VN)
                {
                    vertex.normal = normals[_idx[i].vn - 1];
                }
                else if(_idx[i].type == detail::Index::V_VT_VN)
                {
                    vertex.normal = normals[_idx[i].vn - 1];
                    vertex.uv = uvs[_idx[i].vt - 1];
                }
            }
        }
        /* load material file (path in respect to .obj file) */
        else if(code == "mtllib")
        {
            std::string file;
            ss >> file;
            materials = materialLoad( filepath.substr(0, filepath.find_last_of("\\/")) + "/" + file );
        }
        /* switch to material for next face definitions */
        else if(code == "usemtl")
        {
            auto& model = models.back();
            std::string name;
            ss >> name;

            if(!model.material.empty())
            {
                auto& material = model.material.back();
                material.indexCount = glVertices.size() - material.indexOffset;
            }

            auto& material = model.material.emplace_back( materials[name] );
            material.indexOffset = glVertices.size();
        }
    }

    /* finnish up last object */
    Model& model = models.back();
    model.mesh = meshCreate(glVertices, glIndices);
    if(!model.material.empty())
    {
        auto& material = model.material.back();
        material.indexCount = glVertices.size() - material.indexOffset;
    }

    return models;
}

void modelDelete(std::vector<Model> &models)
{
    for(auto& m : models)
    {
        meshDelete(m.mesh);
    }
}

void modelDelete(Model &model)
{
    meshDelete(model.mesh);
}
