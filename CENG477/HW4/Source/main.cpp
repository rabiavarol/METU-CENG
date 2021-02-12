#include "helper.h"

using namespace std;

static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor = 10.0f;
int displacement = 0;

glm::vec3 lightPos;
GLuint depthMapFBO;
GLuint depthCubemap;
bool lightPosFlag = false;

/****** OUR GLOBALS ******/

// Camera globals
glm::vec3 cameraGaze = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraLeft = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraPos;
GLfloat cameraSpeed = 0.0f;
bool inFullScreenMode = false;
GLFWmonitor* monitor;
const GLFWvidmode* vidMode;


// Matrix globals
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
glm::mat4 MVPMatrix;

// vertex array
glm::vec3* vertexArray;

/*************************/

void setCamera()
{
    // world space -> camera space
    viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraGaze, cameraUp);
    // for modeling transformations
    modelMatrix = glm::mat4(1.0f);
    // for perspective projection (camera space -> clip space)
    projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);

    MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
    
}

static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // change yaw with A and D keys around up vector
    else if(key == GLFW_KEY_A)
    {
        cameraGaze = glm::rotate(cameraGaze, 0.05f, cameraUp);
        cameraLeft = glm::rotate(cameraLeft, 0.05f, cameraUp);
        normalize(cameraGaze); normalize(cameraLeft);
    }
    else if(key == GLFW_KEY_D)
    {
        cameraGaze = glm::rotate(cameraGaze, -0.05f, cameraUp);
        cameraLeft = glm::rotate(cameraLeft, -0.05f, cameraUp);
        normalize(cameraGaze); normalize(cameraLeft);

    }
    // change pitch with W and S keys around left vector
    else if(key == GLFW_KEY_W)
    {
        cameraGaze = glm::rotate(cameraGaze, 0.05f, cameraLeft);
        cameraUp = glm::rotate(cameraUp, 0.05f, cameraLeft);
        normalize(cameraGaze); normalize(cameraUp);
    }
    else if(key == GLFW_KEY_S)
    {
        cameraGaze = glm::rotate(cameraGaze, -0.05f, cameraLeft);
        cameraUp = glm::rotate(cameraUp, -0.05f, cameraLeft);
        normalize(cameraGaze); normalize(cameraUp);

    }
    else if(key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if(inFullScreenMode)
        {
            inFullScreenMode = false;
            glfwSetWindowMonitor(win, NULL, 1000, 1000, widthWindow, heightWindow, GLFW_DONT_CARE);
        }
        else
        {
            inFullScreenMode = true;
            glfwSetWindowMonitor(window, monitor, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);

        }
    }
    else if(key == GLFW_KEY_R)
    {
      heightFactor += 0.5; 
    }

    else if(key == GLFW_KEY_F)
    {
      heightFactor -= 0.5;
    }    
      
    else if(key == GLFW_KEY_Q)
    {
       displacement -= 1;
    }

    else if(key == GLFW_KEY_E)
    {
      displacement += 1; 
    }

    else if(key == GLFW_KEY_T)
    {
      lightPos.y += 5;
    }

    else if(key == GLFW_KEY_G)
    {
      lightPos.y -= 5;
    }

    else if(key == GLFW_KEY_UP)
    {
      lightPos.z += 5;
    }

    else if(key == GLFW_KEY_DOWN)
    {
      lightPos.z -= 5; 
    }

    else if(key == GLFW_KEY_LEFT)
    {
      lightPos.x += 5;
    }

    else if(key == GLFW_KEY_RIGHT)
    {
      lightPos.x -= 5;
    }

    else if(key == GLFW_KEY_Y)
    {
      cameraSpeed += 0.01;
    }

    else if(key == GLFW_KEY_H)
    {
      cameraSpeed -= 0.01;
    }

    else if(key == GLFW_KEY_X)
    {
      cameraSpeed = 0.0f;
    }

    else if(key == GLFW_KEY_I)
    {
		cameraSpeed = 0.0f; 
		cameraGaze = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraLeft = glm::vec3(-1.0f, 0.0f, 0.0f);		
		setCamera();		
		heightFactor = 10.0f;
		displacement = 0;		
		cameraPos = glm::vec3((float)textureWidth/2, (float)textureWidth/10, (float)-textureWidth/4);
		lightPos = glm::vec3((float)textureWidth/2, 100.0f, (float)textureHeight/2);
    }

    
}

static void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    widthWindow = width;
    heightWindow = height;
    glViewport(0, 0, widthWindow, heightWindow);
}

void setMonitor()
{
    monitor = glfwGetPrimaryMonitor();
    vidMode = glfwGetVideoMode(monitor);
    glfwGetWindowSize(win, &widthWindow, &heightWindow);
}

void setUniformVariables()
{
    // Uniform variables must be set by the main program 
    GLint ModelViewProjectionLoc = glGetUniformLocation(idProgramShader, "MVP");
    glUniformMatrix4fv(ModelViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    GLint cameraPosLoc = glGetUniformLocation(idProgramShader, "cameraPosition");
    glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

    GLint lightPosLoc = glGetUniformLocation(idProgramShader, "lightPosition");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    GLint heightFactorPos = glGetUniformLocation(idProgramShader, "heightFactor");
    glUniform1f(heightFactorPos, heightFactor);

    GLuint displacementLoc = glGetUniformLocation(idProgramShader , "displacement");
    glUniform1i(displacementLoc, displacement);

    // set texture uniforms
    GLuint rgbTextureLoc = glGetUniformLocation(idProgramShader, "rgbTexture");
    glUniform1i(rgbTextureLoc, 0);

    GLuint heightMapLoc = glGetUniformLocation(idProgramShader, "heightMap");
    glUniform1i(heightMapLoc, 1);
     
    GLuint textureWidthLoc = glGetUniformLocation(idProgramShader, "widthTexture");
    glUniform1i(textureWidthLoc, textureWidth);

    GLuint textureHeightLoc = glGetUniformLocation(idProgramShader , "heightTexture");
    glUniform1i(textureHeightLoc, textureHeight);

}

void clearAll()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClearStencil(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void createMeshes()
{
    vertexArray = new glm::vec3[6 * textureWidth * textureHeight]; // 2 * w * h -> triangles, each has 3 vertices
    
    int idx = 0;
    for(int i = 0; i < textureWidth; i++)
    {
        for(int j = 0; j < textureHeight; j++)
        {
            vertexArray[idx++] = glm::vec3(i,   0, j);     
            vertexArray[idx++] = glm::vec3(i+1, 0, j+1);   
            vertexArray[idx++] = glm::vec3(i,   0, j+1); 
            vertexArray[idx++] = glm::vec3(i,   0, j);     
            vertexArray[idx++] = glm::vec3(i+1, 0, j); 
            vertexArray[idx++] = glm::vec3(i+1, 0, j+1);   
        }
    }

}

void renderFrame()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6 * textureWidth * textureHeight);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Please provide height and texture image files!\n");
        exit(-1);
    }

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(-1);
    }

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This might be used for local

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    // Window Operations
    glfwMakeContextCurrent(win);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetWindowSizeCallback(win, windowSizeCallback);
    glViewport(0, 0, widthWindow, heightWindow);

    createMeshes();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

        glfwTerminate();
        exit(-1);
    }

    string vs = "shader.vert", fs = "shader.frag";
    initShaders(idProgramShader, vs, fs);
    glUseProgram(idProgramShader);
    initTexture(argv[1], argv[2], &textureWidth, &textureHeight);

    cameraPos = glm::vec3((float)textureWidth/2, (float)textureWidth/10, (float)-textureWidth/4);
    lightPos = glm::vec3((float)textureWidth/2, 100.0f, (float)textureHeight/2);

    setCamera();
    setMonitor();
    setUniformVariables();
    createMeshes();

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(win))
    {
        clearAll();
        cameraPos = cameraPos + cameraSpeed * cameraGaze;
        glViewport(0, 0, widthWindow, heightWindow);
        setCamera();
        setUniformVariables();

        renderFrame();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
