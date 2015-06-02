#version 330 core

in vec4 inVelocity;
in int gl_VertexID; // current index

out vec4 outValue;

uniform samplerBuffer samplerPosition;
uniform usamplerBuffer samplerOtherIndex;
//uniform samplerBuffer samplerLengthToOther;

void main()
{
	outValue = inVelocity;

	vec3 summedForce = vec3(0,0,0);
	int total = 0;
	//int i=0;
	for(int i=0; i<4; ++i){

		int otherIndex = int( texelFetch(samplerOtherIndex, gl_VertexID*4 + i).x );

		if(otherIndex == 108){
			continue;
		}
		++total;

		float targetLen = 3.5; //still needs targetLen
		vec3 pos1 = texelFetch(samplerPosition, gl_VertexID).xyz;
		vec3 pos2 = texelFetch(samplerPosition, int(otherIndex)).xyz;

		vec3 deltaVec = pos1 - pos2;
		float len = length(deltaVec);
		float difference = len - targetLen;
		outValue.a = difference;
		float k=0.02;
		float strength = k * difference;
		//float strength = pow( difference , 1/3f);
		summedForce += -deltaVec/len * strength;
		//outValueOther -= deltaVec/len * strength;
	}
	if(total != 0){
		outValue += vec4(summedForce / total, 0);
		//outValue += inVelocity*0.0000001; // inVelocity must be used
	}
}
