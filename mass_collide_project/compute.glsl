#version 330
layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inVelocity;

out vec4 outValue;

void main() {
	//outValue = inPosition + 0.01;
    outValue.x = inPosition.x - 0.02;
	outValue.y = inPosition.y;
	outValue.z = inPosition.z;
	outValue.a = inPosition.a;
}