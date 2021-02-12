#include "helper.h"

double length(const Vec3f& vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

Vec3f normalize(const Vec3f& vec)
{
    Vec3f nvec;
    double len = length(vec);

    nvec.x = vec.x / len;
    nvec.y = vec.y / len;
    nvec.z = vec.z / len;

    return nvec;
}

Vec3f crossProduct(const Vec3f& v1, const Vec3f& v2)
{
    Vec3f v3;

    v3.x = v1.y*v2.z-v2.y*v1.z;
    v3.y = v2.x*v1.z-v1.x*v2.z;
    v3.z = v1.x*v2.y-v2.x*v1.y;

    return v3;
}

double dotProductVec3f(const Vec3f& v1, const Vec3f& v2)
{
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

double dotProductVec4f(const Vec4f& v1, const Vec4f& v2)
{
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w;
}

Vec3f add(const Vec3f& v1, const Vec3f& v2)
{
    Vec3f v3;

    v3.x = v1.x + v2.x;
    v3.y = v1.y + v2.y;
    v3.z = v1.z + v2.z;

    return v3;
}

Vec3f subtract(const Vec3f& v1, const Vec3f& v2)
{
    Vec3f v3;

    v3.x = v1.x-v2.x;
    v3.y = v1.y-v2.y;
    v3.z = v1.z-v2.z;

    return v3;
}

Vec3f multiplyWithConst(double c, const Vec3f& vec)
{
    Vec3f cvec;
    cvec.x = vec.x * c;
    cvec.y = vec.y * c;
    cvec.z = vec.z * c;

    return cvec;
}

Vec4f convertVec3fToVec4f(const Vec3f& p)
{
    Vec4f res;
    res.x = p.x; res.y = p.y; 
    res.z = p.z;res.w = 1;

    return res;
}

Vec3f convertVec4fToVec3f(const Vec4f& p)
{
    Vec3f res;
    res.x = p.x; res.y = p.y; 
    res.z = p.z; 

    return res;
}

void convertToIdentityMatrix(Matrix4d& m)
{
    m.matrix[0].x = 1; m.matrix[1].y = 1;
    m.matrix[2].z = 1; m.matrix[3].w = 1;
}

Vec4f multiplyMatrixWithPoint(const Matrix4d& M, const Vec4f& P)
{
    Vec4f P_prime;
    P_prime.x = dotProductVec4f(M.matrix[0], P);
    P_prime.y = dotProductVec4f(M.matrix[1], P);
    P_prime.z = dotProductVec4f(M.matrix[2], P);
    P_prime.w = 1;

    return P_prime;
}

Matrix4d multiplyMatrices(const Matrix4d& M1, const Matrix4d& M2)
{
    Matrix4d res;
    for(int i = 0; i < 4; i++)
    {
        Vec4f col2 = {M2.matrix[0].x, M2.matrix[1].x, M2.matrix[2].x, M2.matrix[3].x};
        res.matrix[i].x = dotProductVec4f(M1.matrix[i], col2);
        col2 = {M2.matrix[0].y, M2.matrix[1].y, M2.matrix[2].y, M2.matrix[3].y};
        res.matrix[i].y = dotProductVec4f(M1.matrix[i], col2);
        col2 = {M2.matrix[0].z, M2.matrix[1].z, M2.matrix[2].z, M2.matrix[3].z};
        res.matrix[i].z = dotProductVec4f(M1.matrix[i], col2);
        col2 = {M2.matrix[0].w, M2.matrix[1].w, M2.matrix[2].w, M2.matrix[3].w};
        res.matrix[i].w = dotProductVec4f(M1.matrix[i], col2);
    }
    
    return res;
}

Vec3f negateVector(const Vec3f& vector)
{
	Vec3f negateVector;
	negateVector.x = vector.x*(-1);
	negateVector.y = vector.y*(-1);
	negateVector.z = vector.z*(-1);
	return negateVector;
}