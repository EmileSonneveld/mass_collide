#version 330 core

in vec4 inVelocity;
//in int gl_VertexID; // current index

out vec4 outValue;

uniform samplerBuffer samplerPosition;
uniform usamplerBuffer samplerOtherIndex;
uniform samplerBuffer samplerLengthToOther;

#define MAX_CONNECTIONS %data.MAX_CONNECTIONS%
#define EMPTY_VALUE %data.EMPTY_VALUE%

void main()
{
	outValue = inVelocity;

	vec3 summedForce = vec3(0,0,0);
	float total = 0;
	for(int i=0; i<MAX_CONNECTIONS; ++i){

		int otherIndex = int( texelFetch(samplerOtherIndex, gl_VertexID*MAX_CONNECTIONS + i).x );

		if(otherIndex == EMPTY_VALUE)
			continue;

		float targetLen = texelFetch(samplerLengthToOther, gl_VertexID*MAX_CONNECTIONS + i).x;
		//float targetLen = texelFetch(samplerLengthToOther, otherIndex).x;
		vec3 pos1 = texelFetch(samplerPosition, gl_VertexID).xyz;
		vec3 pos2 = texelFetch(samplerPosition, otherIndex).xyz;

		vec3 deltaVec = pos1 - pos2;
		float len = length(deltaVec);
		if(len == 0)
			continue;
		float difference = len - targetLen;
		
		if(len<0.001 || abs(difference)<0.01)
			continue;
		
		float strength = -1.4 * difference;

		summedForce += normalize (deltaVec) * strength;
		//outValue.a = float(difference); // debug output
		++total;
	}
	if(total > 0){
		summedForce = summedForce/total;
		outValue += vec4(summedForce, 0);
	}
}
