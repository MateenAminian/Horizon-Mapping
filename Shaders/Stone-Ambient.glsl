#version 450

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec2 vertexTexcoord;

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

layout(binding = 0) uniform sampler2D diffuseTexture;

layout(location = 32) uniform vec4 fparam[2];

out vec4 fragmentColor;

void main()
{
	vec3 diffuseColor = fparam[0].xyz;

	// Multiply texture color by ambient light color.

	fragmentColor.xyz = texture(diffuseTexture, vertexTexcoord).xyz * diffuseColor * ambientColor.xyz;
	fragmentColor.w = 0.0;
}
