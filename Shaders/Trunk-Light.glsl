#version 450

in vec3 vertexPosition;				// The interpolated world-space vertex position.
in vec3 vertexNormal;				// The interpolated world-space vertex normal.
in vec4 vertexTangent;				// The interpolated world-space vertex tangent in xyz. Handedness in w.
in vec2 vertexTexcoord;				// The interpolated texture coordinates.

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
layout(binding = 1) uniform sampler2D normalTexture;

layout(location = 32) uniform vec4 fparam[2];

out vec4 fragmentColor;				// The final output color. Set the alpha component (w coordinate) to zero.

void main()
{
	vec3	m;

	m.xy = texture(normalTexture, vertexTexcoord).xy;
	m.z = sqrt(1.0F - m.x * m.x - m.y * m.y);

	// These are the material properties provided by the C++ code plus texturing.

	vec3 diffuseColor = texture(diffuseTexture, vertexTexcoord).xyz;

	vec3 normal = normalize(vertexNormal);
	vec3 tangent = normalize(vertexTangent.xyz);
	vec3 bitangent = cross(normal, tangent) * vertexTangent.w;

	m = tangent * m.x + bitangent * m.y + normal * m.z;

	// Calculate direction to light, get its squared length, and then normalize it.

	vec3 ldir = lightPosition.xyz - vertexPosition;
	float r2 = dot(ldir, ldir);
	ldir *= inversesqrt(r2);

	// Calculate direction to camera and halfway vector.

	vec3 vdir = normalize(cameraPosition.xyz - vertexPosition);
	vec3 hdir = normalize(ldir + vdir);

	// Calculate light attenuation using squared distance to light.

	float atten = clamp(exp(r2 * attenConst.x) * attenConst.y - attenConst.z, 0.0, 1.0);

	// Calculate Lambertian diffuse factor sat(N * L) / pi.

	vec3 diff = diffuseColor * clamp(dot(m, ldir), 0.0, 1.0) * 0.3183;

	// Multiply diffuse color by attenuated light color.

	fragmentColor.xyz = diff * lightColor.xyz * atten;
	fragmentColor.w = 0.0;
}
