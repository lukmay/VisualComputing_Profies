#pragma once

#include "mesh.h"

struct Material
{
    std::string name;

    Vector3D emission;
    Vector3D ambient;
    Vector3D diffuse;
    Vector3D specular;
    float shininess;

    unsigned int indexOffset;
    unsigned int indexCount;
};

struct Model
{
    Mesh mesh;
    std::string name;
    std::vector<Material> material;
};

std::vector<Model> modelLoad(const std::string &filepath);
void modelDelete(std::vector<Model>& models);
void modelDelete(Model& model);
