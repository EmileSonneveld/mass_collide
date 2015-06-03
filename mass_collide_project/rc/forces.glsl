#version 330 core

in vec4 inPosition;
in vec4 inVelocity;
//layout(location = 2) in vec4 inIndexes;

out vec4 outValue;

uniform vec3 point; // contains padding

void main() {
	outValue = inVelocity;
	outValue *= 0.96;
	outValue += vec4(0, -0.002, 0, 0); // Gravity
	//outValue.a = 4310217; // velocity debug value
	if(inPosition.y<-10 && outValue.y < 0){
		outValue.y *= -0.9;
		outValue.y -= 0.01;
	}

	vec3 delta = point - vec3(inPosition.x, inPosition.y, inPosition.z);
	float len = length(delta);
	if (len < 5){
		outValue += vec4(delta, 0) / len * 0.01;
	}
}
