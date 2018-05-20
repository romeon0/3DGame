#version 330 core
layout(location=0) in vec3 position;
uniform vec3 color;
out vec3 inColor;
void main(){
	inColor=color;
	gl_Position = vec4(position,1.0f);
}