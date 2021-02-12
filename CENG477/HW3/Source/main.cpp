#include <iostream>
#include "parser.h"
#include "helper.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace parser;
using namespace std;
//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;
int win_width = scene.camera.image_width;
int win_height = scene.camera.image_height;

double lastTime;
int nbFrames;

int face_count = 0;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void initScene()
{
    glClearColor(scene.background_color.x, scene.background_color.y, scene.background_color.z, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if(scene.culling_enabled)
    {
        glEnable(GL_CULL_FACE);
        if(scene.culling_face) 
            glCullFace(GL_FRONT);
        else 
            glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void setLights()
{
    glEnable(GL_LIGHTING);
    GLfloat ambient[] = {scene.ambient_light.x, scene.ambient_light.y, scene.ambient_light.z, 1.0f};
    
    for(unsigned int i = 0; i < scene.point_lights.size(); i++)
    {
        glEnable(GL_LIGHT0 + i);
        GLfloat curr_intensity[] = {scene.point_lights[i].intensity.x,
                                    scene.point_lights[i].intensity.y,
                                    scene.point_lights[i].intensity.z,
                                    1.0f};
        GLfloat position[] = {scene.point_lights[i].position.x,
                              scene.point_lights[i].position.y,
                              scene.point_lights[i].position.z,
                              1.0f};
        
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, curr_intensity);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, curr_intensity);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, position);
    }
}

void configCamera()
{
    //glViewport(0, 0, win_width, win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(scene.camera.near_plane.x, 
              scene.camera.near_plane.y, 
              scene.camera.near_plane.z, 
              scene.camera.near_plane.w, 
              scene.camera.near_distance, 
              scene.camera.far_distance);
              
    Vec3f eye = scene.camera.position;
    Vec3f at = {eye.x + scene.camera.near_distance * scene.camera.gaze.x,
                eye.y + scene.camera.near_distance * scene.camera.gaze.y,
                eye.z + scene.camera.near_distance * scene.camera.gaze.z};
    Vec3f up = scene.camera.up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);

}

void showFPS(GLFWwindow *pWindow)
{
    double currTime = glfwGetTime();
    double delta = currTime - lastTime;
    char ss[500] = {};
    nbFrames++;
    if(delta >= 1.0)
    {
        double fps = ((double)(nbFrames)) / delta;
        sprintf(ss, "CENG477 - HW3 [%lf FPS]", fps);
        glfwSetWindowTitle(pWindow, ss);

        nbFrames = 0;
        lastTime = currTime;

    }
}

void setTotalFaceCount()
{
    for(unsigned int i = 0; i < scene.meshes.size(); i++)
    {
        for(unsigned int j = 0; j < scene.meshes[i].faces.size(); j++)
        {
            face_count++;
            scene.meshes[i].faces[j].normal = getNormal(scene.vertex_data[scene.meshes[i].faces[j].v0_id-1],
                                                              scene.vertex_data[scene.meshes[i].faces[j].v1_id-1],
                                                              scene.vertex_data[scene.meshes[i].faces[j].v2_id-1]);                       
        }

    }
}

void loadDataToGPU()
{    
    // init buffers
    glGenBuffers(1, &gpuVertexBuffer);
    glGenBuffers(1, &gpuNormalBuffer);
    glGenBuffers(1, &gpuIndexBuffer);

    GLfloat* vertices = new GLfloat[scene.vertex_data.size() * 3];
    GLfloat* normals  = new GLfloat[scene.vertex_data.size() * 3];
    GLint*   indices  = new GLint[face_count * 3];

    for(int i = 0; i < 3*scene.vertex_data.size(); i++)
    {
        normals[i] = 0;
    }
    int v_ind = 0;
    for(unsigned int i = 0; i < scene.vertex_data.size(); i++)
    {
        vertices[v_ind++] = scene.vertex_data[i].x;
        vertices[v_ind++] = scene.vertex_data[i].y;
        vertices[v_ind++] = scene.vertex_data[i].z;
    }
    int i_ind = 0, cnt = 0;
    for(unsigned int i = 0; i < scene.meshes.size(); i++)
    {
        for(unsigned int j = 0; j < scene.meshes[i].faces.size(); j++)
        {
            indices[3*cnt] = scene.meshes[i].faces[j].v0_id-1;
            indices[3*cnt+1] = scene.meshes[i].faces[j].v1_id-1;
            indices[3*cnt+2] = scene.meshes[i].faces[j].v2_id-1;
            cnt++;    
            normals[indices[3*i_ind]*3] += scene.meshes[i].faces[j].normal.x;
            normals[indices[3*i_ind]*3+1] += scene.meshes[i].faces[j].normal.y;
            normals[indices[3*i_ind]*3+2] += scene.meshes[i].faces[j].normal.z;   
            normals[indices[3*i_ind+1]*3] += scene.meshes[i].faces[j].normal.x;
            normals[indices[3*i_ind+1]*3+1] += scene.meshes[i].faces[j].normal.y;
            normals[indices[3*i_ind+1]*3+2] += scene.meshes[i].faces[j].normal.z;
            normals[indices[3*i_ind+2]*3] += scene.meshes[i].faces[j].normal.x;
            normals[indices[3*i_ind+2]*3+1] += scene.meshes[i].faces[j].normal.y;
            normals[indices[3*i_ind+2]*3+2] += scene.meshes[i].faces[j].normal.z;
            i_ind++;
        }
    }

    for(unsigned int i = 0; i < scene.vertex_data.size(); i++)
    {
        Vec3f n;
        n.x = normals[i*3];
        n.y = normals[i*3+1];
        n.z = normals[i*3+2];
        n = normalize(n);
        normals[i*3]   = n.x; 
        normals[i*3+1] = n.y; 
        normals[i*3+2] = n.z; 
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, gpuVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, scene.vertex_data.size()*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, gpuNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, scene.vertex_data.size()*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_count*3*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] vertices;
    delete[] normals;
    delete[] indices;
}

void display()
{
    glClearColor(scene.background_color.x, scene.background_color.y, scene.background_color.z, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //configCamera();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glBindBuffer(GL_ARRAY_BUFFER, gpuVertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, gpuNormalBuffer);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuIndexBuffer);
    
    int offset = 0;
    for(unsigned int i = 0; i < scene.meshes.size(); i++)
    {
        if(scene.meshes[i].mesh_type == "Solid") 
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        Material material = scene.materials[scene.meshes[i].material_id - 1];
        
        GLfloat ambient[] = {material.ambient.x, material.ambient.y, material.ambient.z, 1.0f};
        GLfloat diffuse[] = {material.diffuse.x, material.diffuse.y, material.diffuse.z, 1.0f};
        GLfloat specular[] = {material.specular.x, material.specular.y, material.specular.z, 1.0f};
        GLfloat phong_exp[] = {material.phong_exponent};
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, phong_exp);

        
        glPushMatrix();

        if(scene.meshes[i].transformations.size() != 0)
        {
            for (int j = scene.meshes[i].transformations.size()-1; j >= 0; j--) 
            {
                Transformation trans = scene.meshes[i].transformations[j];

                if (trans.transformation_type == "Translation") 
                {
                    glTranslatef(scene.translations[trans.id-1].x, 
                                 scene.translations[trans.id-1].y, 
                                 scene.translations[trans.id-1].z);
                } 
                else if(trans.transformation_type == "Rotation") 
                {
                    glRotatef(scene.rotations[trans.id-1].x, 
                              scene.rotations[trans.id-1].y, 
                              scene.rotations[trans.id-1].z,
                              scene.rotations[trans.id-1].w);
                } 
                else if(trans.transformation_type == "Scaling") 
                {
                    glScalef(scene.scalings[trans.id-1].x, 
                             scene.scalings[trans.id-1].y, 
                             scene.scalings[trans.id-1].z);
                }
            }
        }
        
        glDrawElements(GL_TRIANGLES, scene.meshes[i].faces.size()*3, 
                       GL_UNSIGNED_INT, (const void*)(offset*sizeof(GLuint)));
        
        offset += scene.meshes[i].faces.size()*3;
        glPopMatrix();
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

}

int main(int argc, char* argv[]) 
{
    scene.loadFromXml(argv[1]);

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);

    // Before Rendering
    initScene();
    setTotalFaceCount();
    loadDataToGPU();
    setLights();
    //configCamera();

    lastTime = glfwGetTime();
    nbFrames = 0;

    while(!glfwWindowShouldClose(win)) 
    {
        glfwWaitEvents();
        showFPS(win);

        configCamera();
        display();
        
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
