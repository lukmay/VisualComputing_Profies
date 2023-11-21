#pragma once

#include <string>

struct Vector2D
{
    float x, y;

    Vector2D(float x = 0, float y = 0);

    Vector2D& operator *=(float s);
    Vector2D& operator /=(float s);

    Vector2D& operator +=(const Vector2D& v);
    Vector2D& operator -=(const Vector2D& v);

     Vector2D operator -() const;

    float& operator [](unsigned int i);
    const float& operator [](unsigned int i) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
};

Vector2D operator *(const Vector2D& v, float s);
Vector2D operator /(const Vector2D& v, float s);
Vector2D operator *(float s, const Vector2D& v);
Vector2D operator /(float s, const Vector2D& v);

Vector2D operator +(const Vector2D& a, const Vector2D& b);
Vector2D operator -(const Vector2D& a, const Vector2D& b);

float length(const Vector2D& v);
Vector2D normalize(const Vector2D& v);

float dot(const Vector2D& a, const Vector2D& b);

Vector2D project(const Vector2D& a, const Vector2D& b);
Vector2D reject(const Vector2D& a, const Vector2D& b);

const std::string toString(const Vector2D& v);
