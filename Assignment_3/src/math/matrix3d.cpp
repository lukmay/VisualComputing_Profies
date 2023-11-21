#include "matrix3d.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <sstream>

#include "matrix4d.h"

Matrix3D::Matrix3D()
{
    n[0][0] = n[0][1] = n[0][2] = 0;
    n[1][0] = n[1][1] = n[1][2] = 0;
    n[2][0] = n[2][1] = n[2][2] = 0;
}

Matrix3D::Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22)
{
    n[0][0] = n00; n[0][1] = n10; n[0][2] = n20;
    n[1][0] = n01; n[1][1] = n11; n[1][2] = n21;
    n[2][0] = n02; n[2][1] = n12; n[2][2] = n22;
}

Matrix3D::Matrix3D(Matrix4D& M)
{
    n[0][0] = M(0,0);   n[0][1] = M(1,0);   n[0][2] = M(2,0);
    n[1][0] = M(0,1);   n[1][1] = M(1,1);   n[1][2] = M(2,1);
    n[2][0] = M(0,2);   n[2][1] = M(1,2);   n[2][2] = M(2,2);
}

Matrix3D Matrix3D::identity()
{
    return Matrix3D( 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 );
}

Matrix3D Matrix3D::scale(float sx, float sy, float sz)
{
    return Matrix3D( sx,  0.0f, 0.0f,
                    0.0f,  sy,  0.0f,
                    0.0f, 0.0f,  sz);
}

Matrix3D Matrix3D::rotationX(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return Matrix3D(1.0f, 0.0f, 0.0f,
                    0.0f,  c,   -s,
                    0.0f,  s,    c  );
}

Matrix3D Matrix3D::rotationY(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return Matrix3D( c,   0.0f,  s,
                    0.0f, 1.0f, 0.0f,
                    -s,   0.0f,  c  );
}

Matrix3D Matrix3D::rotationZ(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return Matrix3D( c,   -s,    0.0f,
                     s,    c,    0.0f,
                     0.0f, 0.0f, 1.0f);
}

Matrix3D Matrix3D::rotation(float r, const Vector3D &a)
{
    float c = std::cos(r);
    float s = std::sin(r);
    float d = 1.0F - c;

    float x = a.x * d;
    float y = a.y * d;
    float z = a.z * d;
    float axay = x * a.y;
    float axaz = x * a.z;
    float ayaz = y * a.z;

    return (Matrix3D(   c + x * a.x,  axay - s * a.z,  axaz + s * a.y,
                     axay + s * a.z,     c + y * a.y,  ayaz - s * a.x,
                        axaz - s * a.y,  ayaz + s * a.x,     c + z * a.z));
}

Vector3D Matrix3D::eulerAngles(const Matrix3D& M)
{
    return Vector3D(
                std::atan2(M(2, 1), M(2, 2)),
                std::atan2(-M(2, 0), std::sqrt(M(2, 1)*M(2, 1) + M(2, 2)*M(2, 2))),
                std::atan2(M(1, 0), M(0, 0))
                );
}

float& Matrix3D::operator ()(int i, int j)
{
    assert(i < 3 && j < 3);
    return n[j][i];
}

const float& Matrix3D::operator ()(int i, int j) const
{
    assert(i < 3 && j < 3);
    return (n[j][i]);
}

Vector3D& Matrix3D::operator [](int j)
{
    assert(j < 3);
    return *reinterpret_cast<Vector3D *>(n[j]);
}

const Vector3D& Matrix3D::operator [](int j) const
{
    assert(j < 3);
    return *reinterpret_cast<const Vector3D *>(n[j]);
}

const float *Matrix3D::ptr() const
{
    return &(n[0][0]);
}

std::ostream& operator<<(std::ostream& os, const Matrix3D& M) {
    os << toString(M);
    return os;
}

Matrix3D operator *(const Matrix3D &A, const Matrix3D &B)
{
    return (Matrix3D(A(0,0) * B(0,0) + A(0,1) * B(1,0) + A(0,2) * B(2,0),
                     A(0,0) * B(0,1) + A(0,1) * B(1,1) + A(0,2) * B(2,1),
                     A(0,0) * B(0,2) + A(0,1) * B(1,2) + A(0,2) * B(2,2),

                     A(1,0) * B(0,0) + A(1,1) * B(1,0) + A(1,2) * B(2,0),
                     A(1,0) * B(0,1) + A(1,1) * B(1,1) + A(1,2) * B(2,1),
                     A(1,0) * B(0,2) + A(1,1) * B(1,2) + A(1,2) * B(2,2),

                     A(2,0) * B(0,0) + A(2,1) * B(1,0) + A(2,2) * B(2,0),
                     A(2,0) * B(0,1) + A(2,1) * B(1,1) + A(2,2) * B(2,1),
                     A(2,0) * B(0,2) + A(2,1) * B(1,2) + A(2,2) * B(2,2)));
}

Vector3D operator *(const Matrix3D &M, const Vector3D &v)
{
    return (Vector3D(M(0,0) * v.x + M(0,1) * v.y + M(0,2) * v.z,
                     M(1,0) * v.x + M(1,1) * v.y + M(1,2) * v.z,
                     M(2,0) * v.x + M(2,1) * v.y + M(2,2) * v.z));
}

Matrix3D inverse(const Matrix3D &M)
{
    const Vector3D& a = M[0];
    const Vector3D& b = M[1];
    const Vector3D& c = M[2];

    Vector3D r0 = cross(b, c);
    Vector3D r1 = cross(c, a);
    Vector3D r2 = cross(a, b);

    float invDet = 1.0F / dot(r2, c);

    return (Matrix3D(r0.x * invDet, r0.y * invDet, r0.z * invDet,
                     r1.x * invDet, r1.y * invDet, r1.z * invDet,
                     r2.x * invDet, r2.y * invDet, r2.z * invDet));
}

const std::string toString(const Matrix3D& M) {
    return std::to_string(M(0, 0)) + " " + std::to_string(M(0, 1)) + " " + std::to_string(M(0, 2)) + "\n"
        + std::to_string(M(1, 0)) + " " + std::to_string(M(1, 1)) + " " + std::to_string(M(1, 2)) + "\n"
        + std::to_string(M(2, 0)) + " " + std::to_string(M(2, 1)) + " " + std::to_string(M(2, 2));
}
