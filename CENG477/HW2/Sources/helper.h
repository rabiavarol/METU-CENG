#ifndef __helper__
#define __helper__

#include <string>
#include <vector>
#include <float.h>
#include "parser.h"

using namespace std;
using namespace parser;

namespace helper
{
    enum objType{ SPHERE, TRIANGLE, MESH, NONE }; // 0, 1, 2, 3

    typedef struct 
    {
        Vec3f origin;
        Vec3f direction;
    } Ray;
    
    typedef struct
    {   
        int objID;
        objType type = NONE;
        double tvalue = DBL_MAX;
        bool isIntersection = false;
        Vec3f surfaceNormal;
        Vec3f intersectionPoint;
        int textureID = 0;
        int currFace = 0;
        pair<float,float> barycentric;
    } Intersection;

    double length(const Vec3f& vec);
    Vec3f normalize(const Vec3f& vec);
    Vec3f crossProduct(const Vec3f& v1, const Vec3f& v2);
    double dotProductVec3f(const Vec3f& v1, const Vec3f& v2);
    double dotProductVec4f(const Vec4f& v1, const Vec4f& v2);
    Vec3f add(const Vec3f& v1, const Vec3f& v2);
    Vec3f subtract(const Vec3f& v1, const Vec3f& v2);
    Vec3f multiplyWithConst(double c, const Vec3f& vec);
    Vec4f convertVec3fToVec4f(const Vec3f& p);
    Vec3f convertVec4fToVec3f(const Vec4f& p);
    void convertToIdentityMatrix(Matrix4d& m);
    Vec4f multiplyMatrixWithPoint(const Matrix4d& M, const Vec4f& P);
    Matrix4d multiplyMatrices(const Matrix4d& M1, const Matrix4d& M2);
    Vec3f negateVector(const Vec3f& vector);
}

#endif

