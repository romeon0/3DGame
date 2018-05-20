#version 330 core
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

in vec2 TexCoords;
in vec3 pos;
out vec4 FragColor;

void main()
{    

			
	FragColor = texture(texture_diffuse1, TexCoords);
	FragColor = vec4(pos, 1.0f);
}