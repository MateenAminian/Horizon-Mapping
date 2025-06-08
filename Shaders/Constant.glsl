#version 450

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec2 vertexTexcoord;

layout(location = 32) uniform vec4 fparam[2];

out vec4 fragmentColor;

void main()
{
	fragmentColor = fparam[0];
}
