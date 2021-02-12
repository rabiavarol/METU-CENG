#include "raytracer.h"
#include <iostream>

bool isMirrorLike(const Material& objMaterial)
{
    return !(objMaterial.mirror.x == 0 && objMaterial.mirror.y == 0 && objMaterial.mirror.z == 0);
}

Ray generateRay(const Camera& cam, int i, int j)
{
    Ray ray;
    ray.origin = cam.position;

    Vec3f u = crossProduct(cam.gaze, cam.up); // u = -w x v   
    u = normalize(u);
    Vec3f v = crossProduct(u, cam.gaze); // v = u x -w 
    v = normalize(v);

    double l = cam.near_plane.x;
    double r = cam.near_plane.y;
    double b = cam.near_plane.z;
    double t = cam.near_plane.w;

    Vec3f lu = multiplyWithConst(l, u);
    Vec3f tv = multiplyWithConst(t, v);
    Vec3f lutv = add(lu, tv);
    
    Vec3f gazedist = multiplyWithConst(cam.near_distance, cam.gaze);
    Vec3f m = add(ray.origin, gazedist); // m = e + gaze*distance
    Vec3f q = add(m, lutv); // q = m + lu + tv

    double su = (i + 0.5) * ((r-l) / cam.image_width);
    double sv = (j + 0.5) * ((t-b) / cam.image_height);

    Vec3f su_u = multiplyWithConst(su, u);
    Vec3f sv_v = multiplyWithConst(sv, v);
    Vec3f tmp = subtract(su_u, sv_v);
    Vec3f s = add(q, tmp); // s = q + su_u - sv_v
    
    ray.direction = subtract(s, ray.origin);
    ray.direction = normalize(ray.direction);
    return ray;
}

Vec3f findIntersectionPoint(const Ray &ray, double t)
{
	Vec3f p;
    // r(t) = o + t*d
	p.x = ray.origin.x + t * ray.direction.x;
	p.y = ray.origin.y + t * ray.direction.y;
	p.z = ray.origin.z + t * ray.direction.z;

	return p;
}

Intersection intersectSphere(const Ray& ray, const Sphere& sphere, const Scene& scene)
{
    Intersection point;
    Vec3f center;
    if(sphere.transformations == ""){
        center = scene.vertex_data[sphere.center_vertex_id-1];
    } else {
        center = convertVec4fToVec3f(transformed_center_vertex_data[sphere.trans_center_vertex_id]);
    }
    float sx = center.x;
    float sy = center.y;
    float sz = center.z;
    
    double C = pow(ray.origin.x-sx,2)+pow(ray.origin.y-sy,2)
        +pow(ray.origin.z-sz,2) - pow(sphere.radius,2);
    double B = 2*ray.direction.x*(ray.origin.x-sx)+2*ray.direction.y*(ray.origin.y-sy)
        +2*ray.direction.z*(ray.origin.z-sz);
    double A = pow(ray.direction.x,2)+pow(ray.direction.y,2)+pow(ray.direction.z,2);
    
    double delta = B*B-4*A*C;
    double t;
    if (delta < 0) return point;
	else if (delta == 0)
	{
		t = -B / (2*A);
	}
	else
	{
		delta = sqrt(delta);
		A = 2*A;
		double t1 = (-B + delta) / A;
		double t2 = (-B - delta) / A;

		if (t2 < t1){
			double tmp = t2;
			t2 = t1;
			t1 = tmp;
		}	
		if (t1 >= 0.0){
            t = t1;
            point.type = SPHERE;
            point.isIntersection = true;
            point.intersectionPoint = findIntersectionPoint(ray, t);
            Vec3f p_minus_c;
            p_minus_c = subtract(point.intersectionPoint, center);
            point.surfaceNormal = normalize(p_minus_c); // (p-c) / R
        } 
		else{
            t = -1;
            point.isIntersection = false;
        } 
	}

    point.tvalue = t;
 	return point;
}

Intersection intersectTriangle(const Ray& ray, const Triangle& tr, const Scene& scene)
{
	Intersection point;
    
	Vec3f ma = convertVec4fToVec3f(transformed_vertex_data[tr.indices.v0_id-1]);
	Vec3f mb = convertVec4fToVec3f(transformed_vertex_data[tr.indices.v1_id-1]);
	Vec3f mc = convertVec4fToVec3f(transformed_vertex_data[tr.indices.v2_id-1]);
	
	double a = ma.x-mb.x;
	double b = ma.y-mb.y;
	double c = ma.z-mb.z;

	double d = ma.x-mc.x;
	double e = ma.y-mc.y;
	double f = ma.z-mc.z;
	
	double g = ray.direction.x;
	double h = ray.direction.y;
	double i = ray.direction.z;
	
	double j = ma.x-ray.origin.x;
	double k = ma.y-ray.origin.y;
	double l = ma.z-ray.origin.z;
	
	double eimhf = e*i-h*f;
	double gfmdi = g*f-d*i;
	double dhmeg = d*h-e*g;
	double akmjb = a*k-j*b;
	double jcmal = j*c-a*l;
	double blmkc = b*l-k*c;

	double M = a*eimhf+b*gfmdi+c*dhmeg;
    if (M == 0) 
		return point;
	
	double t = -(f*akmjb+e*jcmal+d*blmkc)/M;
	if (t < 0.0) 
		return point;
	
	double gamma = (i*akmjb+h*jcmal+g*blmkc)/M;
	if (gamma < 0 || gamma > 1) 
		return point;
	
	double beta = (j*eimhf+k*gfmdi+l*dhmeg)/M;	
	if (beta < 0 || beta > (1-gamma)) 
		return point;

    point.tvalue = t;
    point.type = TRIANGLE;
    point.isIntersection = true;
    point.surfaceNormal = normalize(crossProduct(subtract(mc, mb), subtract(ma, mb)));
    point.intersectionPoint = findIntersectionPoint(ray, t);
    point.barycentric = make_pair(beta,gamma);
	return point;
}

Intersection intersectMesh(const Ray& ray, const Mesh& mesh, const Scene& scene)
{
    bool flag=false; 
    int faceID;
    double tmin = DBL_MAX;
    Intersection point;
    Triangle currTriangle;
    Vec3f normal;
    pair<double,double> barc;
    
    for(int i = 0; i < mesh.faces.size(); i++)
    {
        currTriangle = {mesh.texture_id,
                        mesh.material_id, 
                        mesh.faces[i],
                        mesh.transformations};
        point = intersectTriangle(ray,currTriangle,scene);
        if(point.isIntersection)
        {
            if(point.tvalue > 0.0 && point.tvalue < tmin){
                tmin = point.tvalue;
                flag = point.isIntersection;
                normal = point.surfaceNormal;
                faceID = i;
                barc = point.barycentric;
            }
        }
    }
    point.tvalue = tmin;
    point.isIntersection = flag;
    point.surfaceNormal = normal;
    point.type = MESH;
    point.intersectionPoint = findIntersectionPoint(ray, tmin);
    point.currFace = faceID;
    point.barycentric = barc;
    return point;
}

Intersection findClosestIntersectionsInAll(const Ray& ray, const Scene& scene)
{ 
    Intersection result;         
	// Sphere Intersection
    for(int s = 0; s < scene.spheres.size(); s++)
    {
        Intersection closest = intersectSphere(ray, scene.spheres[s], scene);
        if(closest.isIntersection && closest.tvalue > 0.0 && closest.tvalue < result.tvalue)
        {
            result = closest;
            result.objID = s; 
            result.textureID = scene.spheres[s].texture_id;
        }
    }
    // Triangle Intersection    
    for(int s = 0; s < scene.triangles.size(); s++)
    {
        Intersection closest = intersectTriangle(ray, scene.triangles[s], scene);
        if(closest.isIntersection && closest.tvalue > 0.0 && closest.tvalue < result.tvalue)
        {
            result = closest;
            result.objID = s;
            result.textureID = scene.triangles[s].texture_id;
        }
    }        
	// Mesh Intersection
    for(int s = 0; s < scene.meshes.size(); s++)
    {
        Intersection closest = intersectMesh(ray, scene.meshes[s], scene);
        if(closest.isIntersection && closest.tvalue > 0.0 && closest.tvalue < result.tvalue)
        {
            result = closest;
            result.objID = s;
            result.textureID = scene.meshes[s].texture_id;
        }
    }
    
    return result;
}

Vec3f computeDiffuse(const Intersection& point, int materialID, const PointLight& light, 
                    const Scene& scene, const Vec3f& textureColor)
{
    Vec3f k_d = scene.materials[materialID-1].diffuse;
    Vec3f w_i = subtract(light.position, point.intersectionPoint);
    
    double r_len = pow(w_i.x, 2) + pow(w_i.y, 2) + pow(w_i.z, 2); // r^2 where r = |w_i|
    Vec3f I_d = light.intensity;
    I_d.x /= r_len; I_d.y /= r_len; I_d.z /= r_len; // attenuation

    w_i = normalize(w_i); 
    double wi_dot_n = dotProductVec3f(point.surfaceNormal, w_i); // cos(theta)
    
    if(wi_dot_n < 0) wi_dot_n = 0; // max(0, w_i.n)

	Vec3f diffuseComponent;
	//texture

    if(point.textureID > 0)
    {
    	if(scene.textures[point.textureID-1].decalMode == "replace_all")
    	{
			diffuseComponent = textureColor;
    		return diffuseComponent;
		}
		else if(scene.textures[point.textureID-1].decalMode == "blend_kd")
    	{
			diffuseComponent.x = (k_d.x + textureColor.x/255)/2 * wi_dot_n * I_d.x;
			diffuseComponent.y = (k_d.y + textureColor.y/255)/2 * wi_dot_n * I_d.y;
			diffuseComponent.z = (k_d.z + textureColor.z/255)/2 * wi_dot_n * I_d.z;
    		return diffuseComponent;
    	}	
    	else
    	{
    		k_d.x = textureColor.x/255;
    		k_d.y = textureColor.y/255;
    		k_d.z = textureColor.z/255;
    	}
    }
    diffuseComponent.x = k_d.x * wi_dot_n * I_d.x;
	diffuseComponent.y = k_d.y * wi_dot_n * I_d.y;
	diffuseComponent.z = k_d.z * wi_dot_n * I_d.z;
   
    return diffuseComponent;
}

Vec3f computeSpecular(const Intersection& point, int materialID, const PointLight& light, 
                        const Vec3f& cameraPosition, const Scene& scene)
{
    Vec3f h, specularComponent; // h: half vector
    Vec3f k_s = scene.materials[materialID-1].specular;
    Vec3f w_i = subtract(light.position, point.intersectionPoint);
    Vec3f w_o = subtract(cameraPosition, point.intersectionPoint);

    double r_len = pow(w_i.x, 2) + pow(w_i.y, 2) + pow(w_i.z, 2); // r^2 where r = |w_i|
    Vec3f I_s = light.intensity;
    I_s.x /= r_len; I_s.y /= r_len; I_s.z /= r_len; // attenuation

    w_o = normalize(w_o); 
    w_i = normalize(w_i);
    
    h.x = add(w_i, w_o).x;
    h.y = add(w_i, w_o).y;
    h.z = add(w_i, w_o).z;
    h = normalize(h);

    double n_dot_h = dotProductVec3f(point.surfaceNormal, h); // cos(alpha)
    if(n_dot_h < 0) n_dot_h = 0;
    n_dot_h = pow(n_dot_h, scene.materials[materialID-1].phong_exponent); // n_dot_h ^ p

    specularComponent.x = k_s.x * n_dot_h * I_s.x;
    specularComponent.y = k_s.y * n_dot_h * I_s.y;
    specularComponent.z = k_s.z * n_dot_h * I_s.z;

    return specularComponent;
}

Ray generateReflectionRay(const Intersection& x, const Vec3f& cameraPosition, const Scene& scene)
{
	Vec3f w_0 = normalize(subtract(cameraPosition, x.intersectionPoint));
	Vec3f neg_w_0 = multiplyWithConst(-1,w_0);
	double d = dotProductVec3f(x.surfaceNormal,w_0);

	Vec3f w_r = normalize(add(neg_w_0, multiplyWithConst(2*d,x.surfaceNormal))); 
    Ray reflection;
	reflection.origin = add(x.intersectionPoint, multiplyWithConst(scene.shadow_ray_epsilon, x.surfaceNormal));
	reflection.direction = w_r;

	return reflection;
}