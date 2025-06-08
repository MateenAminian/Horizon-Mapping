#version 450

layout(location = 0) in vec3 positionAttrib;
layout(location = 1) in vec3 normalAttrib;
layout(location = 2) in vec4 tangentAttrib;
layout(location = 3) in vec2 texcoordAttrib;

layout(location = 0) uniform vec4 vparam[7];

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec4 vertexTangent;
out vec2 vertexTexcoord;

layout(std140) uniform universal
{
	vec4	cameraPosition;			// The world-space position of the camera.
	vec4	cameraRight;			// The world-space right direction of the camera.
	vec4	cameraDown;				// The world-space down direction of the camera.
	vec4	ambientColor;			// The constant ambient color in xyz. The w component is not used.
	vec4	lightColor;				// The color of the light in xyz. The w component is not used.
	vec4	lightPosition;			// The world-space position of the light source.
	vec4	attenConst;				// The range of the light source in the x component. The reciprocal range in the y component. The z and w components are not used.
	vec4	fogPlane;				// The world-space fog plane f.
	vec4	fogColor;				// The color of the fog. The w component is not used.
	vec4	fogParams;				// The fog density in x. The value of m from Equation (8.116) in y. The value dot(f, c) in z. The value sgn(dot(f, c)) in w.
};

void main()
{
	// Apply model-view-projection matrix to vertex position.

	gl_Position.x = dot(vparam[0].xyz, positionAttrib) + vparam[0].w;
	gl_Position.y = dot(vparam[1].xyz, positionAttrib) + vparam[1].w;
	gl_Position.z = dot(vparam[2].xyz, positionAttrib) + vparam[2].w;
	gl_Position.w = dot(vparam[3].xyz, positionAttrib) + vparam[3].w;

	// Transform position into world space.

	vec3	P_wld;

	P_wld.x = dot(vparam[4].xyz, positionAttrib) + vparam[4].w;
	P_wld.y = dot(vparam[5].xyz, positionAttrib) + vparam[5].w;
	P_wld.z = dot(vparam[6].xyz, positionAttrib) + vparam[6].w;

	vertexPosition = P_wld;

	// Transform normal into world space. This assumes object-to-world matrix is orthogonal.

	vertexNormal.x = dot(vparam[4].xyz, normalAttrib);
	vertexNormal.y = dot(vparam[5].xyz, normalAttrib);
	vertexNormal.z = dot(vparam[6].xyz, normalAttrib);

	// Transform tangent into world space.

	vertexTangent.x = dot(vparam[4].xyz, tangentAttrib.xyz);
	vertexTangent.y = dot(vparam[5].xyz, tangentAttrib.xyz);
	vertexTangent.z = dot(vparam[6].xyz, tangentAttrib.xyz);
	vertexTangent.w = tangentAttrib.w;

	// Copy texture coordinates.

	vertexTexcoord = texcoordAttrib;
}
