#include "vector2d.h"

#include <cassert>
#include <cmath>
#include <sstream>

Vector2D::Vector2D(float x, float y)
    : x(x), y(y)
{

}

Vector2D Vector2D::operator -() const
{
    return Vector2D(-x, -y);
}


Vector2D& Vector2D::operator *=(float s)
{
    x *= s;
    y *= s;
    return *this;
}

Vector2D& Vector2D::operator /=(float s)
{
    assert(s != 0.0f);
    return *this *= (1.0 / s);
}

Vector2D& Vector2D::operator +=(const Vector2D &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2D& Vector2D::operator -=(const Vector2D &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

float &Vector2D::operator [](unsigned int i)
{
    assert(i < 2);
    return (&x)[i];
}

const float &Vector2D::operator [](unsigned int i) const
{
    assert(i < 2);
    return (&x)[i];
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << toString(v);
    return os;
}

Vector2D operator *(const Vector2D& v, float s)
{
    return Vector2D(v.x * s, v.y * s);
}

Vector2D operator /(const Vector2D& v, float s)
{
    return Vector2D(v.x / s, v.y / s);
}

Vector2D operator *(float s, const Vector2D& v)
{
    return Vector2D(v.x * s, v.y * s);
}

Vector2D operator /(float s, const Vector2D& v)
{
    return Vector2D(v.x / s, v.y / s);
}

Vector2D operator +(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(a.x + b.x, a.y + b.y);
}

Vector2D operator -(const Vector2D& a, const Vector2D& b)
{
    return Vector2D(a.x - b.x, a.y - b.y);
}

float length(const Vector2D &v)
{
    return std::sqrt( v.x*v.x + v.y*v.y );
}

Vector2D normalize(const Vector2D &v)
{
    assert(length(v) != 0.0f);
    return v / length(v);
}

float dot(const Vector2D &a, const Vector2D &b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2D project(const Vector2D &a, const Vector2D &b)
{
   return (b * (dot(a, b) / dot(b, b)));
}

Vector2D reject(const Vector2D &a, const Vector2D &b)
{
    return (a - b * (dot(a, b) / dot(b, b)));
}

const std::string toString(const Vector2D& v) {
    return "x: " +  std::to_string(v.x) + ", y: " + std::to_string(v.y);
}
