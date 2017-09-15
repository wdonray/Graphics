// Phong equation
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

void main()
{
	float lambertTerm = max(0, dot(vec4(-direction, 1), vNormal));
	FragColor = vec4(1) * lambertTerm;
}