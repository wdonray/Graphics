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

vec4 hemi(vec4 skyc, vec4 groundc, vec4 normal)
{
	float NdL = dot(normal, vec4(0,1,0,1));
	float lightin = NdL + 0.5f * 0.5f;
	vec4 result = mix(groundc, skyc, lightin);
	return result;
}
void main()
{
	vec4 N = vNormal;

	float NdL = max(0.0f, dot(direction, vec3(N))); // lambert term
	
	vec3 R = reflect(direction, vec3(N));		    // reflected light vector
	vec3 E = normalize(camPos - vec3(vPosition));  // surface to eye vector

	float specularTerm = max(0.0f ,dot (R, E));
	float specTerm = pow(specularTerm, a);         // specular term
	
	vec3 Ambient = Ka * Ia; // ambient light
	vec3 Diffuse = Kd * Id * NdL;				 // diffused light for one light
	vec3 Specular = Ks * specTerm * Is;			  // specular light for one light

	FragColor = vec4(Ambient + Diffuse + Specular, 1);

	vec4 sky = vec4(0, 0, .5f, 1);
	vec4 ground = vec4(.25, .5f, 0, 1);
	//FragColor = hemi(sky, ground, vNormal);
}