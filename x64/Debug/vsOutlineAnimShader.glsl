#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 boneWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneMatrices[40];
uniform vec3 color;

out vec4 aColor;

void main()
{
	//position
	mat4 boneTransform =  boneMatrices[boneIds[0]] * boneWeights[0];
    boneTransform += boneMatrices[boneIds[1]] 		* boneWeights[1];
    boneTransform += boneMatrices[boneIds[2]] 		* boneWeights[2];
    boneTransform += boneMatrices[boneIds[3]] 		* boneWeights[3];
	
	vec4 localPos = boneTransform*vec4(aPos,1.0); 
    gl_Position = projection * view * model * localPos + vec4(0.0,15.0,0.0,1.0);
	
	aColor = vec4(color,1.0f);
}