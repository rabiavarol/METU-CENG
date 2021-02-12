#include <iostream>
#include <math.h>
#include <cfloat>
#include <cstdlib> 
#include <cstdio> 
#include <fstream>
#include <thread>
#include "helper.h"
#include "parser.h"
#include "ppm.h"
#include <iomanip>
#include "jpeg.h"

typedef unsigned char RGB[3];

using namespace std; 
using namespace parser;
using namespace helper;

vector<Vec4f> transformed_vertex_data;
vector<Vec4f> transformed_center_vertex_data;

pair<double,double> appearance(pair<double,double>& uv, std::string appearance)
{
	if(appearance == "clamp")
	{
		if(uv.first > 1.0) uv.first = 1.0;
		else if(uv.first < 0.0) uv.first = 0.0;
		if(uv.second > 1.0) uv.second = 1.0;
		else if(uv.second < 0.0) uv.second = 0.0;
	}
	else if(appearance == "repeat")
	{
		int fu = floor(uv.first);
		int fv = floor(uv.second);
		uv.first -= fu;
		uv.second -= fv; 
	}
	return uv;
} 

Vec3f interpolateBilinear(const double i, const double j, Vec3f**& texture )
{
	Vec3f rgb;
	int p = floor(i);
	int q = floor(j);
	double dx = i-p;
	double dy = j-q;
	return rgb;
}

Vec3f interpolate(const pair<double,double>& uv, const std::string interpolation,
	const int& width, const int& height, Vec3f**& texture)
{
    
	Vec3f rgb = {0,0,0};
	if(interpolation == "nearest")
    {
		int x = round(uv.first*width);
		int y = round(uv.second*height);
		return texture[y][x];
    }
	else
	{
		int p = floor(uv.first*width);
		int q = floor(uv.second*height);
		double dx = uv.first*width-p;
		double dy = uv.second*height-q;	
		if(p+1 > width - 1 && q+1 > height - 1)
	    {
	        rgb.x = texture[q][p].x*(1-dx)*(1-dy) + texture[q][0].x*dx*(1-dy) 
                    + texture[0][p].x*(1-dx)*dy + texture[0][0].x*dx*dy;

	        rgb.y = texture[q][p].y*(1-dx)*(1-dy) + texture[q][0].y*dx*(1-dy) 
                    + texture[0][p].y*(1-dx)*dy + texture[0][0].y*dx*dy;

	        rgb.z = texture[q][p].z*(1-dx)*(1-dy) + texture[q][0].z*dx*(1-dy) 
                    + texture[0][p].z*(1-dx)*dy + texture[0][0].z*dx*dy;
	    } 
	    else if(p+1 > width - 1)
	    {
	        rgb.x = texture[q][p].x*(1-dx)*(1-dy) + texture[q][0].x*dx*(1-dy)
                    + texture[q+1][p].x*(1-dx)*dy + texture[q+1][0].x*dx*dy;

	        rgb.y = texture[q][p].y*(1-dx)*(1-dy) + texture[q][0].y*dx*(1-dy) 
                    + texture[q+1][p].y*(1-dx)*dy + texture[q+1][0].y*dx*dy;

	        rgb.z = texture[q][p].z*(1-dx)*(1-dy) + texture[q][0].z*dx*(1-dy) 
                    + texture[q+1][p].z*(1-dx)*dy + texture[q+1][0].z*dx*dy;
	    }
	    else if(q+1 > height - 1)
	    {
	        rgb.x = texture[q][p].x*(1-dx)*(1-dy) + texture[q][p+1].x*dx*(1-dy) 
                    + texture[0][p].x*(1-dx)*dy + texture[0][p+1].x*dx*dy;

	        rgb.y = texture[q][p].y*(1-dx)*(1-dy) + texture[q][p+1].y*dx*(1-dy) 
                    + texture[0][p].y*(1-dx)*dy + texture[0][p+1].y*dx*dy;
	 
	        rgb.z = texture[q][p].z*(1-dx)*(1-dy) + texture[q][p+1].z*dx*(1-dy) 
                    + texture[0][p].z*(1-dx)*dy + texture[0][p+1].z*dx*dy;
	    }
	    else
	    {
	        rgb.x = texture[q][p].x*(1-dx)*(1-dy) + texture[q][p+1].x*dx*(1-dy) 
                    + texture[q+1][p].x*(1-dx)*dy + texture[q+1][p+1].x*dx*dy;

	        rgb.y = texture[q][p].y*(1-dx)*(1-dy) + texture[q][p+1].y*dx*(1-dy) 
                    + texture[q+1][p].y*(1-dx)*dy + texture[q+1][p+1].y*dx*dy;
	 
	        rgb.z = texture[q][p].z*(1-dx)*(1-dy) + texture[q][p+1].z*dx*(1-dy) 
                    + texture[q+1][p].z*(1-dx)*dy + texture[q+1][p+1].z*dx*dy;
	    }

	    return rgb;
	}
}

Vec3f mapTriangle(const Scene& scene, const Intersection& intersection, const Triangle& triangle, Vec3f**& texture, pair<int,int> textureWH)
{	
    double beta = intersection.barycentric.first;
	double gamma = intersection.barycentric.second;
	double u = scene.tex_coord_data[triangle.indices.v0_id-1].x + beta*(scene.tex_coord_data[triangle.indices.v1_id-1].x 
            - scene.tex_coord_data[triangle.indices.v0_id-1].x) + gamma*(scene.tex_coord_data[triangle.indices.v2_id-1].x 
            - scene.tex_coord_data[triangle.indices.v0_id-1].x);
	double v = scene.tex_coord_data[triangle.indices.v0_id-1].y + beta*(scene.tex_coord_data[triangle.indices.v1_id-1].y  
            - scene.tex_coord_data[triangle.indices.v0_id-1].y) + gamma*(scene.tex_coord_data[triangle.indices.v2_id-1].y 
            - scene.tex_coord_data[triangle.indices.v0_id-1].y);
	pair<double,double> uv = make_pair(u,v);
	uv = appearance(uv, scene.textures[intersection.textureID-1].appearance);
	Vec3f rgb = interpolate(uv, scene.textures[intersection.textureID-1].interpolation, textureWH.first, textureWH.second, texture);
	
	return rgb;
}

vector<pair<char, int>> parseTransformationStr(std::string str)
{
    vector<pair<char, int>> res;

    for(int i = 0; i < str.length()-1; i += 2)
    {
        pair<char, int> p;
        p.first = str[i++]; 
        p.second = atoi(&str[i]);;
        res.push_back(p); 
    }
    
    return res;
}

Matrix4d applyTranslation(const Scene& scene, int translationID)
{
    Matrix4d T;
    convertToIdentityMatrix(T);
    Vec4f t = {scene.translations[translationID-1].x, scene.translations[translationID-1].y,
               scene.translations[translationID-1].z, 1};
    T.matrix[0].w = t.x; T.matrix[1].w = t.y; T.matrix[2].w = t.z;

    return T;
}

Matrix4d applyRotation(const Scene& scene, int rotationID)
{
    // Alternative Method
    Rotation r = scene.rotations[rotationID-1];
    Vec3f rotationAxis = {r.x, r.y, r.z};
    Vec3f u = normalize(rotationAxis);
    Vec3f absoluted_u = {abs(u.x), abs(u.y), abs(u.z)};
    float min_coor = min(min(absoluted_u.x, absoluted_u.y), absoluted_u.z);

    Vec3f v;
    if(min_coor == absoluted_u.x)
    {
        v = {0, -u.z, u.y};
    } 
    else if(min_coor == absoluted_u.y)
    {
        v = {-u.z, 0, u.x};
    } 
    else
    {
        v = {-u.y, u.x, 0};
    }
    v = normalize(v);
    Vec3f w = normalize(crossProduct(u, v));

    Matrix4d M, M_inv, R_x;
    convertToIdentityMatrix(M); convertToIdentityMatrix(M_inv); convertToIdentityMatrix(R_x);
    
    double sin_theta = sin(r.angle * M_PI/180.0);
	double cos_theta = cos(r.angle * M_PI/180.0);

    R_x.matrix[1].y = cos_theta; R_x.matrix[1].z = -sin_theta;
    R_x.matrix[2].y = sin_theta; R_x.matrix[2].z =  cos_theta;

    M_inv.matrix[0].x = u.x; M_inv.matrix[0].y = v.x; M_inv.matrix[0].z = w.x; 
    M_inv.matrix[1].x = u.y; M_inv.matrix[1].y = v.y; M_inv.matrix[1].z = w.y;
    M_inv.matrix[2].x = u.z; M_inv.matrix[2].y = v.z; M_inv.matrix[2].z = w.z;

    M.matrix[0].x = u.x; M.matrix[0].y = u.y; M.matrix[0].z = u.z; 
    M.matrix[1].x = v.x; M.matrix[1].y = v.y; M.matrix[1].z = v.z;
    M.matrix[2].x = w.x; M.matrix[2].y = w.y; M.matrix[2].z = w.z;
    
    Matrix4d compositeMatrix = multiplyMatrices(M_inv, multiplyMatrices(R_x, M));

    return compositeMatrix;
}

Matrix4d applyScaling(const Scene& scene, int scalingID, float& radius)
{
    Matrix4d S;
    convertToIdentityMatrix(S);
    S.matrix[0].x = scene.scalings[scalingID-1].x;
    S.matrix[1].y = scene.scalings[scalingID-1].y;
    S.matrix[2].z = scene.scalings[scalingID-1].z;
    radius *= S.matrix[0].x;
    return S;
}

void transformTriangle(const Scene& scene, const Triangle& tr)
{
    Vec4f v_0 = convertVec3fToVec4f(scene.vertex_data[tr.indices.v0_id-1]);
    Vec4f v_1 = convertVec3fToVec4f(scene.vertex_data[tr.indices.v1_id-1]);
    Vec4f v_2 = convertVec3fToVec4f(scene.vertex_data[tr.indices.v2_id-1]);
    
    vector<pair<char, int>> trans = parseTransformationStr(tr.transformations);
    Matrix4d combinedTransMatrix;
    convertToIdentityMatrix(combinedTransMatrix);
    
    for(int j = 0; j < trans.size(); j++)
    {
        if(trans[j].first == 't') // translation
        { 
            combinedTransMatrix = multiplyMatrices(applyTranslation(scene, trans[j].second), combinedTransMatrix);
        }
        else if(trans[j].first == 'r') // rotation
        {    
            combinedTransMatrix = multiplyMatrices(applyRotation(scene, trans[j].second), combinedTransMatrix);
        }
        else // scaling
        { 
            float r = 0; // triangle does not have radius
            combinedTransMatrix = multiplyMatrices(applyScaling(scene, trans[j].second, r), combinedTransMatrix);
        }
    }
    
    transformed_vertex_data[tr.indices.v0_id-1] = multiplyMatrixWithPoint(combinedTransMatrix, v_0);
    transformed_vertex_data[tr.indices.v1_id-1] = multiplyMatrixWithPoint(combinedTransMatrix, v_1);
    transformed_vertex_data[tr.indices.v2_id-1] = multiplyMatrixWithPoint(combinedTransMatrix, v_2);

}

void transformSphere(const Scene& scene, Sphere& sp)
{
    Matrix4d translationScalingM, rotationM;
    convertToIdentityMatrix(translationScalingM); 
    convertToIdentityMatrix(rotationM);
    vector<pair<char, int>> trans = parseTransformationStr(sp.transformations);
    
    for(int j = 0; j < trans.size(); j++)
    {
        if(trans[j].first == 't') // translation
        { 
            translationScalingM = multiplyMatrices(applyTranslation(scene, trans[j].second), translationScalingM);
        }
        else if(trans[j].first == 'r') // rotation
        {    
            sp.isRotated = true;
            translationScalingM = multiplyMatrices(applyRotation(scene, trans[j].second),  translationScalingM);
        	rotationM = multiplyMatrices(applyRotation(scene, trans[j].second),  rotationM);
        }
        else // scaling
        { 
            translationScalingM = multiplyMatrices(applyScaling(scene, trans[j].second, sp.radius), translationScalingM);
        }
    }
    
    sp.R = rotationM;
    Vec4f center = convertVec3fToVec4f(scene.vertex_data[sp.center_vertex_id-1]); // original center
    transformed_center_vertex_data.push_back(multiplyMatrixWithPoint(translationScalingM, center)); // center after transformations
}

Matrix4d translateBack(const Vec3f& vector)
{
	Matrix4d T;
    convertToIdentityMatrix(T);
    Vec4f t = {vector.x, vector.y, vector.z, 1};
    T.matrix[0].w = t.x; T.matrix[1].w = t.y; T.matrix[2].w = t.z;

    return T;
}

Matrix4d rotateBack(Rotation r)
{
	Vec3f rotationAxis = {r.x, r.y, r.z};
    Vec3f u = normalize(rotationAxis);
    Vec3f absoluted_u = {abs(u.x), abs(u.y), abs(u.z)};
    float min_coor = min(min(absoluted_u.x, absoluted_u.y), absoluted_u.z);

    Vec3f v;
    if(min_coor == absoluted_u.x)
    {
        v = {0, -u.z, u.y};
    } 
    else if(min_coor == absoluted_u.y)
    {
        v = {-u.z, 0, u.x};
    } 
    else
    {
        v = {-u.y, u.x, 0};
    }
    v = normalize(v);
    Vec3f w = normalize(crossProduct(u, v));

    Matrix4d M, M_inv, R_x;
    convertToIdentityMatrix(M); convertToIdentityMatrix(M_inv); convertToIdentityMatrix(R_x);
    
    double sin_theta = sin(r.angle * M_PI/180.0);
	double cos_theta = cos(r.angle * M_PI/180.0);
    R_x.matrix[1].y = cos_theta; R_x.matrix[1].z = -sin_theta;
    R_x.matrix[2].y = sin_theta; R_x.matrix[2].z =  cos_theta;

    M_inv.matrix[0].x = u.x; M_inv.matrix[0].y = v.x; M_inv.matrix[0].z = w.x; 
    M_inv.matrix[1].x = u.y; M_inv.matrix[1].y = v.y; M_inv.matrix[1].z = w.y;
    M_inv.matrix[2].x = u.z; M_inv.matrix[2].y = v.z; M_inv.matrix[2].z = w.z;

    M.matrix[0].x = u.x; M.matrix[0].y = u.y; M.matrix[0].z = u.z; 
    M.matrix[1].x = v.x; M.matrix[1].y = v.y; M.matrix[1].z = v.z;
    M.matrix[2].x = w.x; M.matrix[2].y = w.y; M.matrix[2].z = w.z;
    
    Matrix4d compositeMatrix = multiplyMatrices(M_inv, multiplyMatrices(R_x, M));

    return compositeMatrix;		
}

Matrix4d scaleBack(Vec3f vector, const float& radius)
{
    Matrix4d S;
    convertToIdentityMatrix(S);
    S.matrix[0].x /= vector.x;
    S.matrix[1].y /= vector.y;
    S.matrix[2].z /= vector.z;
   // radius /= S.matrix[0].x; //neden sadece x ekseni bölünüyor
    return S;
}

Vec3f transformBack(const Scene& scene, const Sphere& sp, const Vec3f& point)
{
	Matrix4d m;
	convertToIdentityMatrix(m);
    vector<pair<char, int>> trans = parseTransformationStr(sp.transformations);
    
    for(int j = trans.size()-1; j >=0 ; j--)
    {
        if(trans[j].first == 'r') // rotation
        {    
        	Rotation backVec = scene.rotations[trans[j].second-1];
        	backVec.angle *= (-1);
        	m = multiplyMatrices(rotateBack(backVec),  m);
        }
       
    }
    
    Vec4f p = multiplyMatrixWithPoint(m, convertVec3fToVec4f(point));
    return convertVec4fToVec3f(p); 
}

pair<double,double> sphereCoordinateUV(const Scene& scene,const Vec4f& center, const Vec3f& point, 
                                        const Sphere& sphere, const int& textureWidth, const int& textureHeigth)
{
	Vec3f tmp = subtract(point, convertVec4fToVec3f(center));
    if(sphere.isRotated)
    {
        tmp = transformBack(scene, sphere, tmp);
    }

    double r = length(tmp);
	double theta = acos(tmp.y/r);
	double phi = atan2(tmp.z , tmp.x);

	double u = ((-1)*phi + M_PI) / (2 * M_PI);
	double v = theta / M_PI;

	return std::make_pair(u,v);
}

void applyTransformations(Scene& scene)
{
    for(int i = 0; i < scene.triangles.size(); i++)
    {
        if(scene.triangles[i].transformations != "")
        {
            transformTriangle(scene, scene.triangles[i]);
        }
            
    }

    for(int i = 0; i < scene.meshes.size(); i++)
    {
        for(int j = 0; j < scene.meshes[i].faces.size(); j++)
        {
            Triangle tr = {scene.meshes[i].texture_id,
                           scene.meshes[i].material_id, 
                           scene.meshes[i].faces[j], 
                           scene.meshes[i].transformations};
            if(tr.transformations != "")
            {
                transformTriangle(scene, tr);
            }
        }
    }
    
    int transCenterVertexID = 0;
    for(int i = 0; i < scene.spheres.size(); i++)
    {
        if(scene.spheres[i].transformations != "")
        {
            scene.spheres[i].trans_center_vertex_id = transCenterVertexID;
            transformSphere(scene, scene.spheres[i]);
            transCenterVertexID++;
        }
            
    }

}

Vec3f findPixelRGB(const Intersection& closestIntersection, const
                    Vec3f& cameraPosition, const Scene& scene, int recursionDepth, Vec3f**& texture, pair<int,int> textureWH)
{
    Vec3f rgb = {0,0,0};
    if(closestIntersection.isIntersection)
    {
        int materialID;
        int textureID;
        Vec3f textureRGB = {0,0,0};
        switch (closestIntersection.type)
        {
            case SPHERE:
            {
				materialID = scene.spheres[closestIntersection.objID].material_id;
                textureID = scene.spheres[closestIntersection.objID].texture_id;
                break;
            } 
            case TRIANGLE:
            {
                materialID = scene.triangles[closestIntersection.objID].material_id;
                textureID = scene.triangles[closestIntersection.objID].texture_id;
                break;
            }
            case MESH:
            {
            	materialID = scene.meshes[closestIntersection.objID].material_id;
                textureID = scene.meshes[closestIntersection.objID].texture_id;
                break;
            }
        }
        
        if(textureID > 0)
        {
        	switch (closestIntersection.type)
		    {
				case SPHERE:
				{
					pair<double,double> uv = sphereCoordinateUV(scene,
						transformed_center_vertex_data[scene.spheres[closestIntersection.objID].trans_center_vertex_id],
						closestIntersection.intersectionPoint, scene.spheres[closestIntersection.objID], textureWH.first, textureWH.second);
				    
                    textureRGB= interpolate(appearance(uv,scene.textures[closestIntersection.textureID-1].appearance), 
                    	scene.textures[closestIntersection.textureID-1].interpolation, textureWH.first, textureWH.second, texture);
					break;
				}	
				case TRIANGLE:
                    textureRGB = mapTriangle(scene, closestIntersection, scene.triangles[closestIntersection.objID], texture, textureWH);
                    break;
				case MESH:
				{
				    Triangle currTriangle = {textureID, materialID, scene.meshes[closestIntersection.objID].faces[closestIntersection.currFace]};
				    textureRGB = mapTriangle(scene, closestIntersection, currTriangle, texture, textureWH);
					break;
				}  
			}
        }
        
		// ambient 
		rgb.x +=  scene.materials[materialID-1].ambient.x*scene.ambient_light.x;
		rgb.y +=  scene.materials[materialID-1].ambient.y*scene.ambient_light.y;
		rgb.z +=  scene.materials[materialID-1].ambient.z*scene.ambient_light.z;	
        
        
    	for(int i = 0; i < scene.point_lights.size(); i++)
        {
            // diffuse 
            Vec3f diff = computeDiffuse(closestIntersection, materialID, scene.point_lights[i], scene, textureRGB);
            // specular
            Vec3f spec = computeSpecular(closestIntersection, materialID, scene.point_lights[i], cameraPosition, scene);

            // shadow ray computation
    		Vec3f toLight = subtract(scene.point_lights[i].position, closestIntersection.intersectionPoint);
            Vec3f toLightNormalized = normalize(toLight);
            // to avoid self intersection:
			Vec3f x = add(closestIntersection.intersectionPoint, multiplyWithConst(scene.shadow_ray_epsilon, closestIntersection.surfaceNormal));
            const Ray s = {x, toLightNormalized};
            
            // intersect shadow ray (s) with an object
            Intersection shadowObj = findClosestIntersectionsInAll(s,scene);
            // distance from x point to the light
            double toLightLength = length(toLight);
            // distance from x point to the intersected objected
            double shadowObjLength = length(subtract(shadowObj.intersectionPoint, x));
		
            // if the intersecting object in front of the light:
            //    point is in shadow – no contribution from this light
			if(shadowObj.isIntersection && shadowObjLength < toLightLength) 
                continue; 
            // else:
            //    the intersecting object behind the light OR there is no object along the shadow ray (s) direction
            rgb.x += diff.x + spec.x;
	        rgb.y += diff.y + spec.y;
	        rgb.z += diff.z + spec.z;

    	}

        // reflection from mirror-like surfaces
        if(recursionDepth > 0 && isMirrorLike(scene.materials[materialID-1]))
        {
            Ray w_r = generateReflectionRay(closestIntersection, cameraPosition, scene);  
            Intersection newIntersectionP = findClosestIntersectionsInAll(w_r, scene);
            if(newIntersectionP.isIntersection)
                {
                Vec3f cameraPositionT = add(multiplyWithConst(length(subtract(cameraPosition,closestIntersection.intersectionPoint)), 
                                            multiplyWithConst(-1,w_r.direction)),closestIntersection.intersectionPoint);

                Vec3f reflectionRGB = findPixelRGB(newIntersectionP, cameraPositionT, scene, recursionDepth-1,texture, textureWH);
                rgb.x += reflectionRGB.x * scene.materials[materialID-1].mirror.x;
                rgb.y += reflectionRGB.y * scene.materials[materialID-1].mirror.y;
                rgb.z += reflectionRGB.z * scene.materials[materialID-1].mirror.z;
            }
        }
                
    }
    else
    {
        rgb.x = scene.background_color.x;
        rgb.y = scene.background_color.y;
        rgb.z = scene.background_color.z;
    }

    return rgb;
}

Vec3f** readTexture(const std::string filename, int &w, int &h)
{
    
	ifstream fin(filename.c_str());
    
    if(!fin.is_open()) {
        cout << "Error: failed to open input file - " << filename << endl;
    }
    string header;
    int width = 0, height = 0, maxVal = 0;
    fin >> header >> width >> height >> maxVal;
    w = width; h = height;
    
    Vec3f** image = new Vec3f* [height];
    for (int j = 0; j < height; j++) 
    {
        image[j] = new Vec3f[width];
    }
    
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) 
        {
            Vec3f colors;
            fin >> colors.x >> colors.y >> colors.z;
            image[i][j] = colors;  
        }
    }
    
    return image;
}

string getTextureFileName(string name)
{
    string filename = "";
    
    for(int i = 0; name[i+9] != '.'; i++)
    {
        filename += name[i+9];
    }

    return filename;
}

string convertJpegToPpm(string imageName)
{
    string outFilename = getTextureFileName(imageName);
    outFilename += ".ppm";
    // Read JPEG file
    int w, h;
    char* i = const_cast<char*>(imageName.c_str());
    read_jpeg_header(i, w, h);
    
    unsigned char* jpegImage = new unsigned char [w * h * 3];
    read_jpeg(i, jpegImage, w, h);
    
    // Now save as PPM
    char* o = const_cast<char*>(outFilename.c_str());
    write_ppm(o, jpegImage, w, h);
    
    return outFilename;
}

void createImageRows(int start, int end, int imageWidth, int camNum, unsigned char* image, const Scene& scene)
{
	Vec3f*** textures = nullptr;
	vector<pair<int,int>> textureWH;
	if(scene.textures.size())
    {
		textures = new Vec3f**[scene.textures.size()];
		for(int i = 0; i < scene.textures.size(); i++)
		{
			string out_ppm_file = convertJpegToPpm(scene.textures[i].imageName);
            
            int width, height;
			textures[i] = readTexture(out_ppm_file, width, height);
			textureWH.push_back(make_pair(width,height));
		}
	}
    // traverse each pixel
    int pixelNum = start * imageWidth * 3;
    for(int j = start; j < end; j++)
    {
        for(int i = 0; i < imageWidth; i++)
        {            	
        	Ray currRay = generateRay(scene.cameras[camNum], i, j);
            
            Intersection closestIntersection = findClosestIntersectionsInAll(currRay, scene);  
			Vec3f rgb;
            pair<int,int> notnecessary;
            // textures?
            if(closestIntersection.textureID > 0)
            	rgb = findPixelRGB(closestIntersection, scene.cameras[camNum].position, 
            		scene, scene.max_recursion_depth, textures[closestIntersection.textureID-1], textureWH[closestIntersection.textureID-1]); 
            else
            {
           		rgb = findPixelRGB(closestIntersection, scene.cameras[camNum].position, 
            		scene, scene.max_recursion_depth, *textures, notnecessary); 
            }
            if(rgb.x > 255) rgb.x = 255;
            if(rgb.y > 255) rgb.y = 255;
            if(rgb.z > 255) rgb.z = 255;
            
            image[pixelNum++] = round(rgb.x);
            image[pixelNum++] = round(rgb.y);
            image[pixelNum++] = round(rgb.z);
        }
    }
}

int main(int argc, char* argv[])
{
    parser::Scene scene;
    scene.loadFromXml(argv[1]);

    transformed_vertex_data.resize(scene.vertex_data.size()); 
    // APPLY TRANSFORMATIONS BEFORE RENDERING
    applyTransformations(scene);
    // RENDERING PART
    for(int c = 0; c < scene.cameras.size(); c++)
    {    
        int numOfThreads = 1;

	    int imageWidth = scene.cameras[c].image_width;
        int imageHeigth = scene.cameras[c].image_height;
        int block_size = imageHeigth / numOfThreads; 

	    unsigned char* image = new unsigned char [imageWidth * imageHeigth * 3];

	 	vector<std::thread> threads;
	 	for(int t = 0; t < numOfThreads; t++){
            int start = t * block_size, end = (t + 1) * block_size;
            threads.push_back(std::thread(createImageRows, start, end, imageWidth, c, std::ref(image), scene));
    	}

        for(int i = 0; i < numOfThreads; i++){
            threads[i].join();
        }

	    write_ppm(scene.cameras[c].image_name.c_str(), image, imageWidth, imageHeigth);
        delete[] image;
    }

    return 0;
}
