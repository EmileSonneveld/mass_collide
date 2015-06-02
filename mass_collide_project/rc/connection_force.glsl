#version 330 core

in vec4 inPosition;
in vec4 inVelocity;
//in uint inOtherIndex; // filled in by inConnectionIndexAlt
in int gl_VertexID; // current index

out vec4 outValue;

uniform samplerBuffer samplerPosition;
uniform samplerBuffer samplerVelocity;

void main()
{
	// proof of concept
	vec4 velocity = vec4(texelFetch(samplerVelocity, gl_VertexID).xyz, 0);
	outValue = inPosition + velocity;

	// vex3 pos1 = texelFetch(samplerPosition, gl_VertexID).xyz;
	// vex3 pos2 = texelFetch(samplerPosition, inOtherIndex).xyz;
	//
	// vec3 deltaVec = pos1 - pos2;
	// float len = length(deltaVec);
	// float difference = len - targetLen; still needs targetLen
	//
	// float k=50;
	// float strength = k * difference;
	//
	// outValue += deltaVec/len * strength;
	// //outValueOther -= deltaVec/len * strength;
}
