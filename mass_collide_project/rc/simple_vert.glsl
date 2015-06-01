#version 150

in vec4 inPosition;
in vec4 inColor;

out vec4 color;

uniform mat4 ViewProjectionMatrix; // no need for model matrix


void main() {


	vec4 tmpPos = inPosition;
	tmpPos.a = 1;
	gl_Position = ViewProjectionMatrix  * tmpPos;

	color = inColor;
	color.a = 1;
}
