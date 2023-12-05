#include "light.h"

Light createLight(){
  Light light;
  setDayLight(light);

  return light;
}

void setDayLight(Light &light){
  light.ambientColor = Vector3D(1.0f,1.0f,1.0f);
  light.color = Vector3D(1.0f,1.0f,1.0f);
  light.direction = Vector3D(1.0f,1.0f,0.0f);
  light.ambientCoeff = 1.0f;
  light.diffuseCoeff = 0.8f;
  light.specularCoeff = 0.8f;
}

void setNightLight(Light &light){
  light.ambientColor = Vector3D(0.4f,0.4f,0.4f);
  light.color = Vector3D(1.0f,1.0f,1.0f);
  light.direction = Vector3D(1.0f,1.0f,0.0f);
  light.ambientCoeff = 0.4f;
  light.diffuseCoeff = 0.4f;
  light.specularCoeff = 0.4f;
}