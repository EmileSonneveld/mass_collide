#version 330 core

in vec4 inPosition;
in vec4 inVelocity;

out vec4 outValue;


void main() {
    outValue.x = inPosition.x + inVelocity.x;
	outValue.y = inPosition.y + inVelocity.y;
	outValue.z = inPosition.z + inVelocity.z;
	outValue.a = inPosition.a;


	if(inPosition.y<-10 && outValue.y < 0){
		outValue.y = -10;
	}
}
