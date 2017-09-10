#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pos;

uniform sampler2D texture_diffuse1;
uniform float drawGrid;

void main()
{    
	if(drawGrid>0.5){
		float xMod = mod(pos.x,2.0);
		float yMod = mod(pos.y,2.0);
		float zMod = mod(pos.z,2.0);
		if(xMod>=0.0 && xMod<=0.1 && pos.y==0.0)
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else if(zMod>=0.0 && zMod<=0.1 && pos.y==0.0)
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else
			FragColor = texture(texture_diffuse1, TexCoords);
	}
	else
		FragColor = texture(texture_diffuse1, TexCoords);
}