#include "light.h"

Light createLight(){
  Light light;
  setDayLight(light);

  return light;
}

void setDayLight(Light &light){
  light.ambientColor = Vector3D(1.0f,0.9f,0.9f);
  light.color = Vector3D(0.7f,0.7f,0.7f);
  light.direction = Vector3D(-1.0f,1.0f,0.0f);
  light.ambientCoeff = 1.0f;
  light.diffuseCoeff = 0.7f;
  light.specularCoeff = 0.3f;
  light.background = Vector3D(0.53f,0.81f,0.95f);
}

void setNightLight(Light &light){
  light.ambientColor = Vector3D(0.6f,0.6f,0.6f);
  light.color = Vector3D(0.7f,0.7f,0.7f);
  light.direction = Vector3D(-1.0f,1.0f,0.0f);
  light.ambientCoeff = 1.0f;
  light.diffuseCoeff = 0.6f;
  light.specularCoeff = 0.4f;
  light.background = Vector3D(0.23f,0.51f,0.80f);
}