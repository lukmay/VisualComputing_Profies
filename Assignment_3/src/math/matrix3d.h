#pragma once

#include "vector3d.h"

struct Matrix4D;

struct Matrix3D
{
    float n[3][3];


    Matrix3D();
    Matrix3D(float n00, float n01, float n02,
             float n10, float n11, float n12,
             float n20, float n21, float n22);
    Matrix3D(Matrix4D& m);

    static Matrix3D identity();
    static Matrix3D scale(float sx, float sy, float sz);
    static Matrix3D rotationX(float r);
    static Matrix3D rotationY(float r);
    static Matrix3D rotationZ(float r);
    static Matrix3D rotation(float r, const Vector3D& a);
    static Vector3D eulerAngles(const Matrix3D& m);

    float& operator ()(int i, int j);
    const float& operator ()(int i, int j) const;
    Vector3D& operator [](int j);
    const Vector3D& operator [](int j) const;
    const float* ptr() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix3D& M);
};

Matrix3D operator *(const Matrix3D& A, const Matrix3D& B);
Vector3D operator *(const Matrix3D& M, const Vector3D& v);

Matrix3D inverse(const Matrix3D& M);

const std::string toString(const Matrix3D& M);
