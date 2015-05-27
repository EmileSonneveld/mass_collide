#version 150

in vec4 inPosition;

uniform mat4 ViewProjectionMatrix; // no need for model matrix


void main() {
	vec4 tmpPos = inPosition;
	tmpPos.a = 1;
	gl_Position = ViewProjectionMatrix  * tmpPos;
}
