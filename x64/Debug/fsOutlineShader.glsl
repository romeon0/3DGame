#version 330 core
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

in vec2 TexCoords;
in vec3 pos;
out vec4 FragColor;
in vec3 Color;

void main()
{
    FragColor = vec4(Color,1.0f);//vec4(0.0, 0.0, 0.0, 0.0f);
}