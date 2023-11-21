#pragma once

#include "vector3d.h"

struct Vector4D
{
    float x, y, z, w;


    Vector4D(const Vector3D& v, float w = 1.0f);
    Vector4D(float x = 0, float y = 0, float z = 0, float w = 0);

    Vector4D& operator *=(float s);
    Vector4D& operator /=(float s);

    Vector4D& operator +=(const Vector4D& v);
    Vector4D& operator -=(const Vector4D& v);

    Vector4D operator -() const;

    float& operator [](unsigned int i);
    const float& operator [](unsigned int i) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector4D& v);
};

Vector4D operator *(const Vector4D& v, float s);
Vector4D operator /(const Vector4D& v, float s);
Vector4D operator *(float s, const Vector4D& v);
Vector4D operator /(float s, const Vector4D& v);

Vector4D operator +(const Vector4D& a, const Vector4D& b);
Vector4D operator -(const Vector4D& a, const Vector4D& b);

const std::string toString(const Vector4D& v);
