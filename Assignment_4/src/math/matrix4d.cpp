#include "matrix4d.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <sstream>

Matrix4D::Matrix4D()
{
    n[0][0] = n[0][1] = n[0][2] = n[0][3] = 0;
    n[1][0] = n[1][1] = n[1][2] = n[1][3] = 0;
    n[2][0] = n[2][1] = n[2][2] = n[2][3] = 0;
    n[3][0] = n[3][1] = n[3][2] = n[3][3] = 0;
}

Matrix4D::Matrix4D(float n00, float n01, float n02, float n03,
                   float n10, float n11, float n12, float n13,
                   float n20, float n21, float n22, float n23,
                   float n30, float n31, float n32, float n33)
{
    n[0][0] = n00; n[0][1] = n10; n[0][2] = n20; n[0][3] = n30;
    n[1][0] = n01; n[1][1] = n11; n[1][2] = n21; n[1][3] = n31;
    n[2][0] = n02; n[2][1] = n12; n[2][2] = n22; n[2][3] = n32;
    n[3][0] = n03; n[3][1] = n13; n[3][2] = n23; n[3][3] = n33;
}

Matrix4D::Matrix4D(const Matrix3D &M)
{
    n[0][0] = M(0,0);   n[0][1] = M(1,0);   n[0][2] = M(2,0);   n[0][3] = 0;
    n[1][0] = M(0,1);   n[1][1] = M(1,1);   n[1][2] = M(2,1);   n[1][3] = 0;
    n[2][0] = M(0,2);   n[2][1] = M(1,2);   n[2][2] = M(2,2);   n[2][3] = 0;
    n[3][0] = 0;        n[3][1] = 0;        n[3][2] = 0;        n[3][3] = 1;
}

Matrix4D Matrix4D::identity()
{
    return Matrix4D(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}

Matrix4D Matrix4D::scale(float sx, float sy, float sz)
{
    return Matrix4D(Matrix3D::scale(sx, sy, sz));
}

Matrix4D Matrix4D::rotationX(float r)
{
    return Matrix4D(Matrix3D::rotationX(r));
}

Matrix4D Matrix4D::rotationY(float r)
{
    return Matrix4D(Matrix3D::rotationY(r));
}

Matrix4D Matrix4D::rotationZ(float r)
{
    return Matrix4D(Matrix3D::rotationZ(r));
}

Matrix4D Matrix4D::rotation(float r, const Vector3D& a)
{
    return Matrix4D(Matrix3D::rotation(r, a));
}

Matrix4D Matrix4D::translation(const Vector3D &v)
{
    return Matrix4D(1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0,  1  );
}

Matrix4D Matrix4D::perspective(float fov, float aspect, float nearPlane, float farPlane)
{
    float f = 1.0f / std::tan(0.5 * fov);
    float c1 = -(farPlane + nearPlane) / (farPlane - nearPlane);
    float c2 = -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane);

    return Matrix4D(f/aspect,   0,  0,  0,
                    0,          f,  0,  0,
                    0,          0,  c1, c2,
                    0,          0,  -1,  0);
}

Matrix4D Matrix4D::ortho(float left, float bottom, float right, float top, float near, float far)
{
    return Matrix4D(
                2.0f / (right - left),  0.0f,                   0.0f,                   -(right+left)/(right-left),
                0.0f,                   2.0f / (top - bottom),  0.0f,                   -(top+bottom)/(top-bottom),
                0.0f,                   0.0f,                   -2.0f / (far - near),   -(far+near)/(far-near),
                0.0f,                   0.0f,                   0.0f,                   1.0f
                );
}

float& Matrix4D::operator ()(int i, int j)
{
    assert(i < 4 && j < 4);
    return n[j][i];
}

Vector4D &Matrix4D::operator [](int j)
{
    assert(j < 4);
    return *reinterpret_cast<Vector4D *>(n[j]);
}

const float *Matrix4D::ptr() const
{
    return &(n[0][0]);
}

std::ostream& operator<<(std::ostream& os, const Matrix4D& M) {
    os << toString(M);
    return os;
}

const Vector4D& Matrix4D::operator [](int j) const
{
    assert(j < 4);
    return *reinterpret_cast<const Vector4D *>(n[j]);
}

const float& Matrix4D::operator ()(int i, int j) const
{
    assert(i < 4 && j < 4);
    return n[j][i];
}

Matrix4D operator *(const Matrix4D& A, const Matrix4D& B)
{
    return Matrix4D(A(0,0) * B(0,0) + A(0,1) * B(1,0) + A(0,2) * B(2,0) + A(0,3) * B(3,0),
                    A(0,0) * B(0,1) + A(0,1) * B(1,1) + A(0,2) * B(2,1) + A(0,3) * B(3,1),
                    A(0,0) * B(0,2) + A(0,1) * B(1,2) + A(0,2) * B(2,2) + A(0,3) * B(3,2),
                    A(0,0) * B(0,3) + A(0,1) * B(1,3) + A(0,2) * B(2,3) + A(0,3) * B(3,3),

                    A(1,0) * B(0,0) + A(1,1) * B(1,0) + A(1,2) * B(2,0) + A(1,3) * B(3,0),
                    A(1,0) * B(0,1) + A(1,1) * B(1,1) + A(1,2) * B(2,1) + A(1,3) * B(3,1),
                    A(1,0) * B(0,2) + A(1,1) * B(1,2) + A(1,2) * B(2,2) + A(1,3) * B(3,2),
                    A(1,0) * B(0,3) + A(1,1) * B(1,3) + A(1,2) * B(2,3) + A(1,3) * B(3,3),

                    A(2,0) * B(0,0) + A(2,1) * B(1,0) + A(2,2) * B(2,0) + A(2,3) * B(3,0),
                    A(2,0) * B(0,1) + A(2,1) * B(1,1) + A(2,2) * B(2,1) + A(2,3) * B(3,1),
                    A(2,0) * B(0,2) + A(2,1) * B(1,2) + A(2,2) * B(2,2) + A(2,3) * B(3,2),
                    A(2,0) * B(0,3) + A(2,1) * B(1,3) + A(2,2) * B(2,3) + A(2,3) * B(3,3),

                    A(3,0) * B(0,0) + A(3,1) * B(1,0) + A(3,2) * B(2,0) + A(3,3) * B(3,0),
                    A(3,0) * B(0,1) + A(3,1) * B(1,1) + A(3,2) * B(2,1) + A(3,3) * B(3,1),
                    A(3,0) * B(0,2) + A(3,1) * B(1,2) + A(3,2) * B(2,2) + A(3,3) * B(3,2),
                    A(3,0) * B(0,3) + A(3,1) * B(1,3) + A(3,2) * B(2,3) + A(3,3) * B(3,3));
}

Vector4D operator *(const Matrix4D& M, const Vector4D& v)
{
    return Vector4D(M(0,0) * v[0] + M(0,1) * v[1] + M(0,2) * v[2] + M(0,3) * v[3],
                    M(1,0) * v[0] + M(1,1) * v[1] + M(1,2) * v[2] + M(1,3) * v[3],
                    M(2,0) * v[0] + M(2,1) * v[1] + M(2,2) * v[2] + M(2,3) * v[3],
                    M(3,0) * v[0] + M(3,1) * v[1] + M(3,2) * v[2] + M(3,3) * v[3]);
}

Matrix4D inverse(const Matrix4D &M)
{
    const Vector3D& a = reinterpret_cast<const Vector3D&>(M[0]);
    const Vector3D& b = reinterpret_cast<const Vector3D&>(M[1]);
    const Vector3D& c = reinterpret_cast<const Vector3D&>(M[2]);
    const Vector3D& d = reinterpret_cast<const Vector3D&>(M[3]);

    const float& x = M(3,0);
    const float& y = M(3,1);
    const float& z = M(3,2);
    const float& w = M(3,3);

    Vector3D s = cross(a, b);
    Vector3D t = cross(c, d);
    Vector3D u = a * y - b * x;
    Vector3D v = c * w - d * z;

    float invDet = 1.0f / (dot(s, v) + dot(t, u));
    s *= invDet;
    t *= invDet;
    u *= invDet;
    v *= invDet;

    Vector3D r0 = cross(b, v) + t * y;
    Vector3D r1 = cross(v, a) - t * x;
    Vector3D r2 = cross(d, u) + s * w;
    Vector3D r3 = cross(u, c) - s * z;

    return (Matrix4D(r0.x, r0.y, r0.z, -dot(b, t),
                     r1.x, r1.y, r1.z,  dot(a, t),
                     r2.x, r2.y, r2.z, -dot(d, s),
                     r3.x, r3.y, r3.z,  dot(c, s)));
}

const std::string toString(const Matrix4D& M) {
    return std::to_string(M(0, 0)) + " " + std::to_string(M(0, 1)) + " " + std::to_string(M(0, 2)) + " " + std::to_string(M(0,3)) + "\n"
        + std::to_string(M(1, 0)) + " " + std::to_string(M(1, 1)) + " " + std::to_string(M(1, 2)) + " " + std::to_string(M(1,3)) + "\n"
        + std::to_string(M(2, 0)) + " " + std::to_string(M(2, 1)) + " " + std::to_string(M(2, 2)) + " " + std::to_string(M(2,3)) + "\n"
        + std::to_string(M(3, 0)) + " " + std::to_string(M(3, 1)) + " " + std::to_string(M(3, 2)) + " " + std::to_string(M(3,3));
}

