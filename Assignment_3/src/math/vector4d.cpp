#include "vector4d.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <sstream>

Vector4D::Vector4D(const Vector3D &v, float w)
    : x(v.x), y(v.y), z(v.z), w(w)
{

}

Vector4D::Vector4D(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
{

}

Vector4D Vector4D::operator -() const
{
    return Vector4D(-x, -y, -z, -w);
}

Vector4D &Vector4D::operator *=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

Vector4D& Vector4D::operator /=(float s)
{
    assert(s != 0.0f);
    return *this *= (1.0 / s);
}

Vector4D &Vector4D::operator +=(const Vector4D &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return *this;
}

Vector4D &Vector4D::operator -=(const Vector4D &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;
}

float &Vector4D::operator [](unsigned int i)
{
    assert(i < 4);
    return ((&x)[i]);
}

const float &Vector4D::operator [](unsigned int i) const
{
    assert(i < 4);
    return ((&x)[i]);
}

std::ostream& operator<<(std::ostream& os, const Vector4D& v) {
    os << toString(v);
    return os;
}

Vector4D operator *(const Vector4D &v, float s)
{
    return Vector4D(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4D operator /(const Vector4D &v, float s)
{
    return Vector4D(v.x / s, v.y / s, v.z / s, v.w / s);
}

Vector4D operator *(float s, const Vector4D &v)
{
    return Vector4D(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vector4D operator /(float s, const Vector4D &v)
{
    return Vector4D(v.x / s, v.y / s, v.z / s, v.w / s);
}

Vector4D operator +(const Vector4D &a, const Vector4D &b)
{
    return Vector4D(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4D operator -(const Vector4D &a, const Vector4D &b)
{
    return Vector4D(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

const std::string toString(const Vector4D& v) {
    return "x: " +  std::to_string(v.x) + ", y: " + std::to_string(v.y) + ", z: " + std::to_string(v.z) + ", w: " + std::to_string(v.w);
}

