#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D texture_diffuse1;

void main(){
	float dropColor = 20.0/255.0;
	FragColor = texture(texture_diffuse1, TexCoords);
	if (FragColor.a < 0.4)
    {
        discard; 
    }
	else if(FragColor.r>dropColor &&  FragColor.g>dropColor && FragColor.b>dropColor)
		discard;
	//FragColor = vec4(0.4,0.3,0.2,1.0f);
	
}
