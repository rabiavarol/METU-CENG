#include "parser.h"

using namespace parser;

double length(const Vec3f& vec)
{
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
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

Vec3f getNormal(const Vec3f& a, const Vec3f& b, const Vec3f& c) 
{
    Vec3f a_minus_b = subtract(a, b);
    Vec3f c_minus_b = subtract(c, b);
    return normalize(crossProduct(c_minus_b, a_minus_b));
}