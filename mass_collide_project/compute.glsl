#version 330 core

in vec4 inPosition;
in vec4 inVelocity;

out vec4 outValue;


void main() {
	//outValue = inPosition + 0.01;
    outValue.x = inPosition.x + inVelocity.x;
	outValue.y = inPosition.y + inVelocity.y;
	outValue.z = inPosition.z + inVelocity.z;
	outValue.a = inPosition.a;
}
