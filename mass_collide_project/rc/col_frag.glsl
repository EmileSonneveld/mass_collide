#version 330 core

// Interpolated values from the vertex shaders
in vec4 outColor;

// Ouput data
out vec4 finalColor;

uniform sampler2D TextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
    finalColor = outColor;
}
