#version 330 core

//in vec4 inPosition;
in vec4 inVelocity;
//in uint inOtherIndex; // filled in by inConnectionIndexAlt
//in float inLengthToOther;
in int gl_VertexID; // current index

out vec4 outValue;

uniform samplerBuffer samplerPosition;
uniform usamplerBuffer samplerOtherIndex;
//uniform samplerBuffer samplerVelocity;

void main()
{
	vec3 summedForce = vec3(0,0,0);
	int i=0;
	//for(int i=0; i<1; ++i){

		uint other = texelFetch(samplerOtherIndex, gl_VertexID*4 + i).x;
		//outValue = vec4(float(other.x), float(other.y), float(other.z), float(other.a));
		outValue = vec4(float(other), float(other), float(other), float(other));
		int otherIndex = int(texelFetch(samplerOtherIndex, gl_VertexID + i).x);

		if(otherIndex == 108){
			outValue.a = 108108f;
			return;
			//break;
		}

		float targetLen = 6; //still needs targetLen
		vec3 pos1 = texelFetch(samplerPosition, gl_VertexID).xyz;
		vec3 pos2 = texelFetch(samplerPosition, int(otherIndex)).xyz;
		//outValue = vec4(pos1, 666);
		//outValue = vec4(float(gl_VertexID), float(otherIndex), float(i), 50505050);
		vec3 deltaVec = pos1 - pos2;
		float len = length(deltaVec);
		float difference = len - targetLen;

		float k=-0.2;
		float strength = k * difference;

		summedForce += deltaVec/len * strength;
		//outValueOther -= deltaVec/len * strength;
	//}
	//outValue = inVelocity + vec4(summedForce / 4, 0);
	//outValue = outValue * 0.99 + vec4(0, -0.002, 0, 0);
	outValue += inVelocity*0.0000001; // inVelocity must be used
}
