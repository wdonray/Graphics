// Hemi equation
#version 410
uniform vec3 direction;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 Ia;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float a;

in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;

out vec4 FragColor;
uniform vec3 camPos; // world space camera pos

vec4 hemi(vec4 skyc, vec4 groundc, vec4 normal)
{
	float NdL = dot(normal, vec4(0,1,0,1));
	float lightin = NdL + 0.5f * 0.5f;
	vec4 result = mix(groundc, skyc, lightin);
	return result;
}

void main()
{

	vec4 sky = vec4(0, 0.25f, 1, 1);
	vec4 ground = vec4(0, .5f, 0, 1);
	FragColor = hemi(sky, ground, normalize(vNormal));
}