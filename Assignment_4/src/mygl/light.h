#pragma once

#include <math/vector3d.h>

struct Light
{
  Vector3D ambientColor;
  Vector3D color;
  Vector3D direction;
  float ambientCoeff;
  float diffuseCoeff;
  float specularCoeff;
  Vector3D background;
};

Light createLight();

void setDayLight(Light &light);

void setNightLight(Light &light);