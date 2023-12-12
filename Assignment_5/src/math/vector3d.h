#pragma once

#include <string>

struct Vector4D;

struct Vector3D
{
    float x, y, z;


    Vector3D(float x = 0, float y = 0, float z = 0);
    Vector3D(const Vector4D& v);

    Vector3D& operator *=(float s);
    Vector3D& operator /=(float s);

    Vector3D& operator +=(const Vector3D& v);
    Vector3D& operator -=(const Vector3D& v);

    Vector3D operator -() const;

    float& operator [](unsigned int i);
    const float& operator [](unsigned int i) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v);
};

Vector3D operator *(const Vector3D& v, float s);
Vector3D operator /(const Vector3D& v, float s);
Vector3D operator *(float s, const Vector3D& v);
Vector3D operator /(float s, const Vector3D& v);

Vector3D operator +(const Vector3D& a, const Vector3D& b);
Vector3D operator -(const Vector3D& a, const Vector3D& b);

float length(const Vector3D& v);
Vector3D normalize(const Vector3D& v);

float dot(const Vector3D& a, const Vector3D& b);
Vector3D cross(const Vector3D& a, const Vector3D& b);

Vector3D project(const Vector3D& a, const Vector3D& b);
Vector3D reject(const Vector3D& a, const Vector3D& b);

const std::string toString(const Vector3D& v);
