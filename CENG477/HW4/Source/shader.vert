#version 410

layout(location = 0) in vec3 position;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix

uniform vec3 cameraPosition;
uniform float heightFactor;
uniform vec3 lightPosition;
uniform int displacement; 


// Texture-related data
uniform sampler2D rgbTexture;
uniform sampler2D heightMap;
uniform int widthTexture;
uniform int heightTexture;

// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

void main()
{

   textureCoordinate = vec2(1 - (position.x + displacement)/ widthTexture, 
      1 - position.z / heightTexture);
   vec4 textureColor = texture(heightMap,textureCoordinate);   
   vec3 tempPos = vec3(position.x, textureColor.x*heightFactor, position.z);

   vec3 neighbors[6] = vec3[6](
      vec3(position.x + 1.0f + displacement,   0, position.z), //right
      vec3(position.x + 1.0f + displacement,   0, position.z - 1.0f), //bottomRigth
      vec3(position.x + displacement,          0, position.z - 1.0f), // bottom
      vec3(position.x - 1.0f + displacement,   0, position.z), //left
      vec3(position.x - 1.0f + displacement,   0, position.z + 1.0f), //topleft
      vec3(position.x + displacement,          0, position.z + 1.0f)); //top

   for(int i=0; i<6; i++)
   {
         vec2 coord = vec2(1 - neighbors[i].x / widthTexture, 1 - neighbors[i].z / heightTexture);
         vec4 color = texture(heightMap, coord);
         neighbors[i].y = heightFactor * color.x;
   }

   vec3 normalSum = vec3(0.0f, 0.0f, 0.0f);
   for(int i=0; i<5; i++)
   {
      normalSum += cross(neighbors[i] - tempPos, neighbors[i+1] - tempPos);
   }
   normalSum += cross(neighbors[5] - tempPos, neighbors[0] - tempPos);

   vertexNormal = normalize(normalSum);
   ToLightVector = normalize(lightPosition - tempPos);
   ToCameraVector = normalize(cameraPosition - tempPos);
   gl_Position = MVP * vec4(tempPos, 1.0); 
}