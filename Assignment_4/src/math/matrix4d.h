#pragma once

#include "matrix3d.h"
#include "vector4d.h"


struct Matrix4D
{
    float n[4][4];

    Matrix4D();
    Matrix4D(float n00, float n01, float n02, float n03,
             float n10, float n11, float n12, float n13,
             float n20, float n21, float n22, float n23,
             float n30, float n31, float n32, float n33);

    Matrix4D(const Vector4D& a, const Vector4D& b, const Vector4D& c, const Vector4D& d);
    Matrix4D(const Matrix3D& M);

    static Matrix4D identity();
    static Matrix4D scale(float sx, float sy, float sz);
    static Matrix4D rotationX(float r);
    static Matrix4D rotationY(float r);
    static Matrix4D rotationZ(float r);
    static Matrix4D rotation(float r, const Vector3D& a);
    static Matrix4D translation(const Vector3D& v);
    static Matrix4D perspective(float fov, float aspect, float nearPlane, float farPlane);
    static Matrix4D ortho(float left, float bottom, float right, float top, float nearPlane, float farPlane);

    float& operator ()(int i, int j);
    const float& operator ()(int i, int j) const;
    Vector4D& operator [](int j);
    const Vector4D& operator [](int j) const;
    const float* ptr() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix4D& M);
};

Matrix4D operator *(const Matrix4D& A, const Matrix4D& B);
Vector4D operator *(const Matrix4D& M, const Vector4D& v);

Matrix4D inverse(const Matrix4D& M);

const std::string toString(const Matrix4D& M);
