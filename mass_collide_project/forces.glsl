#version 330 core

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inVelocity;
//layout(location = 2) in vec4 inIndexes;

out vec4 outValue;

uniform vec3 point; // contains padding

void main() {
	outValue = inVelocity * 0.99 + vec4(0, -0.002, 0, 0);
	if(inPosition.y<-10)
		outValue.y *= -0.9;

	vec3 delta = point - vec3(inPosition.x, inPosition.y, inPosition.z);
	float len = length(delta);
	if (len < 50){
		outValue += vec4(delta, 0) / len * 0.01;
	}
}
