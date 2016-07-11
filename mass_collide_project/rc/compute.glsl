#version 330 core

in vec4 inPosition;
in vec4 inVelocity;

out vec4 outValue;

// move position with velocity

const int groundHeight = -2;

void main() {
    outValue.x = inPosition.x + inVelocity.x;
	outValue.y = inPosition.y + inVelocity.y;
	outValue.z = inPosition.z + inVelocity.z;
	outValue.a = inPosition.a;


	if(inPosition.y<groundHeight && outValue.y < 0){
		outValue.y = groundHeight;
	}
}
