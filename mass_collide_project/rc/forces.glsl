#version 330 core

in vec4 inPosition;
in vec4 inVelocity;
//layout(location = 2) in vec4 inIndexes;

out vec4 outValue; // new Velocity

uniform vec3 point; // contains padding

const int groundHeight = -4;

void main() {
	outValue = inVelocity;
	outValue *= 0.9;
	outValue += vec4(0, -0.0025 / %data.ini.connection_force_itterations%, 0, 0); // Gravity
	//outValue.a = 4310217; // velocity debug value
	if(inPosition.y < groundHeight && outValue.y < 0){
		outValue.y *= -0.9999;
		//outValue.y -= 0.005;
	}

	vec3 delta = point - vec3(inPosition.x, inPosition.y, inPosition.z); // mouse
	float len = length(delta);
	if (len < 5){
		outValue += vec4(delta, 0) / len * 0.003;
	}
}
