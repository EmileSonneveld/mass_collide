#version 330 core

in vec4 inVelocity;
in int gl_VertexID; // current index

out vec4 outValue;

uniform samplerBuffer samplerPosition;
uniform usamplerBuffer samplerOtherIndex;
uniform samplerBuffer samplerLengthToOther;

void main()
{
	outValue = inVelocity;

	vec3 summedForce = vec3(0,0,0);
	int total = 0;
	//int i=0;
	for(int i=0; i<4; ++i){

		int otherIndex = int( texelFetch(samplerOtherIndex, gl_VertexID*4 + i).x );

		if(otherIndex == 108){
			//outValue.a = 898;
			continue;
		}
		++total;

		float targetLen = texelFetch(samplerLengthToOther, gl_VertexID*4 + i).x;
		vec3 pos1 = texelFetch(samplerPosition, gl_VertexID).xyz;
		vec3 pos2 = texelFetch(samplerPosition, otherIndex).xyz;

		vec3 deltaVec = pos1 - pos2;
		float len = length(deltaVec);
		if(len == 0) continue;
		float difference = len - targetLen;
		float k=-0.2;
		float strength = k * difference;
		//float strength = pow( difference , 1/3f);
		summedForce += deltaVec/len * strength;
		//outValueOther -= deltaVec/len * strength;

		outValue.a = float(difference); // debug output
	}
	if(total != 0){
		outValue += vec4(summedForce / float(total), 0);
		//outValue += inVelocity*0.0000001; // inVelocity must be used
	}
}
