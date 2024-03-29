#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 cubeVertices;
layout(location = 1) in vec4 inPosition; // Contains size too!
layout(location = 2) in vec4 inColor;

// Output data ; will be interpolated for each pixel.
out vec4 outColor;


// Values that stay constant for the whole mesh.
//uniform vec3 CameraRight_worldspace;
//uniform vec3 CameraUp_worldspace;
// Model-View-Projection matrix, but without the Model
// (the position is in BillboardPos; the orientation depends on the camera)
uniform mat4 ViewProjMatrix;

void main()
{
	// gl_InstanceID is usable here
    float particleSize = inPosition.w; // because we encoded it this way.
    vec3 particleCenter_wordspace = inPosition.xyz;

	vec3 vertexPosition_worldspace =
		particleCenter_wordspace + cubeVertices * particleSize;

	gl_Position = ViewProjMatrix * vec4(vertexPosition_worldspace, 1.0f);
	outColor = inColor;
}
