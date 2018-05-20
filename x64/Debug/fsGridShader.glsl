#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 pos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform float drawGrid;

void main()
{    
	/*if(drawGrid>0.5){
		float xMod = mod(pos.x,2.0);
		float yMod = mod(pos.y,2.0);
		float zMod = mod(pos.z,2.0);
		if(xMod>=0.0 && xMod<=0.1)
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else if(zMod>=0.0 && zMod<=0.1)            // && pos.y<=0.1
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else
			FragColor = texture(texture_diffuse1, TexCoords);
	}
	else
		FragColor = texture(texture_diffuse1, TexCoords);*/
		
		
		
	if(drawGrid>0.5){
		float xMod = abs(mod(pos.x,0.0363636));
		float yMod = abs(mod(pos.y,0.0363636));
		float zMod = abs(mod(pos.z,0.0363636));
	
		if(xMod>=0.000 && xMod <= 0.001) 
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else if(zMod>=0.000 && zMod <= 0.001) 
			FragColor=vec4(0.9,0.1,0.8,1.0);
		else
			FragColor = texture(texture_diffuse1, TexCoords);
	}
	else
		FragColor = texture(texture_diffuse1, TexCoords);
}