#include "vector3d.h"

#include "vector4d.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <sstream>

Vector3D::Vector3D(float x, float y, float z)
    : x(x), y(y), z(z)
{

}

Vector3D::Vector3D(const Vector4D& v)
    : x(v.x), y(v.y), z(v.z)
{

}

Vector3D Vector3D::operator -() const
{
    return Vector3D(-x, -y, -z);
}

Vector3D& Vector3D::operator *=(float s)
{
    x *= s;
    y *= s;
    z *= s;

    return *this;
}

Vector3D& Vector3D::operator /=(float s)
{
    assert(s != 0.0f);
    return *this *= (1.0 / s);
}

Vector3D& Vector3D::operator +=(const Vector3D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vector3D& Vector3D::operator -=(const Vector3D &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

float& Vector3D::operator [](unsigned int i)
{
    assert(i < 3);
    return (&x)[i];
}

const float& Vector3D::operator [](unsigned int i) const
{
    assert(i < 3);
    return (&x)[i];
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
    os << toString(v);
    return os;
}

Vector3D operator *(const Vector3D &v, float s)
{
    return Vector3D(v.x * s, v.y * s, v.z * s);
}

Vector3D operator /(const Vector3D &v, float s)
{
    return Vector3D(v.x / s, v.y / s, v.z / s);
}

Vector3D operator *(float s, const Vector3D &v)
{
    return Vector3D(v.x * s, v.y * s, v.z * s);
}

Vector3D operator /(float s, const Vector3D &v)
{
    return Vector3D(v.x / s, v.y / s, v.z / s);
}

float length(const Vector3D &v)
{
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3D normalize(const Vector3D &v)
{
    assert(length(v) != 0.0f);
    return v / length(v);
}

Vector3D operator +(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D operator -(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

float dot(const Vector3D &a, const Vector3D &b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3D cross(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
                );
}

Vector3D project(const Vector3D &a, const Vector3D &b)
{
    return (b * (dot(a, b) / dot(b, b)));
}

Vector3D reject(const Vector3D &a, const Vector3D &b)
{
    return (a - b * (dot(a, b) / dot(b, b)));
}

const std::string toString(const Vector3D& v) {
    return "x: " +  std::to_string(v.x) + ", y: " + std::to_string(v.y) + ", z: " + std::to_string(v.z);
}
