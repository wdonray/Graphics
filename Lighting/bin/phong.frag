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
uniform vec3 camPos; // world space camera pos
uniform sampler2D image;
uniform float time;


vec3 Ambient(vec3 Ka, vec3 Ia)
{
	return Ka * Ia;
}

vec3 Diffuse(vec3 Kd, vec3 Id, vec3 L, vec3 N)
{
	float NdL = max(0.0f, dot(L, N));

	return Kd * Id * NdL;
}

vec3 Specular(vec3 Ks, vec3 Is, vec3 L, vec3 N, vec3 camPosition, vec3 position)
{
	vec3 R = reflect(L, N);
	vec3 E = normalize(camPosition - position);

	float specularTerm = pow(max(0.0f ,dot (R, E)), a);
	 
	return Ks * Is * specularTerm;
}

void main()
{
	vec3 N = normalize(vNormal.xyz);

	vec3 ambient = Ambient(Ka, Ia);

	vec3 diffuse = Diffuse(Kd, Id, normalize(direction), -N);

	vec3 specular = Specular(Ks, Is, normalize(direction), N, camPos, vPosition.xyz);
	vec2 duv = vUV;
	duv.x +=time / 20;
	FragColor = vec4(ambient + diffuse + specular, 1) * texture(image, duv);
}
