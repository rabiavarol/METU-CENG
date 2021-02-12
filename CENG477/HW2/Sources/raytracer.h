#ifndef __raytracer_h__
#define __raytracer_h__

#include "parser.h"

bool isMirrorLike(const Material& objMaterial);
Ray generateRay(const Camera& cam, int i, int j);
Vec3f findIntersectionPoint(const Ray &ray, double t);
Intersection intersectSphere(const Ray& ray, const Sphere& sphere, const Scene& scene);
Intersection intersectTriangle(const Ray& ray, const Triangle& tr, const Scene& scene);
Intersection intersectMesh(const Ray& ray, const Mesh& mesh, const Scene& scene);
Intersection findClosestIntersectionsInAll(const Ray& ray, const Scene& scene);
Vec3f computeDiffuse(const Intersection& point, int materialID, const PointLight& light, 
                    const Scene& scene, const Vec3f& textureColor);
Vec3f computeSpecular(const Intersection& point, int materialID, const PointLight& light, 
                        const Vec3f& cameraPosition, const Scene& scene);
Ray generateReflectionRay(const Intersection& x, const Vec3f& cameraPosition, const Scene& scene);

#endif // __raytracer_h__
