#version 330 core

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inVelocity;

out vec4 outValue;


void main() {
	outValue = inVelocity * 0.99 + vec4(0, -0.002, 0, 0);
	if(inPosition.y<-10)
		outValue.y *= -0.9;

}
