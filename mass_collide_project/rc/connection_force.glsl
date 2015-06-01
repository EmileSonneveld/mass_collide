#version 330 core

in vec4 inPosition;
in vec4 inVelocity;
//in uint inOtherIndex; // filled in by inConnectionIndexAlt

uniform vec4 buffPosition[120];
uniform vec4 buffVelocity[120];

// http://stackoverflow.com/questions/7954927/glsl-passing-a-list-of-values-to-fragment-shader

out vec4 outValue;


void main()
{
	outValue = buffPosition[gl_VertexID];
}
