#version 410
in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;

out vec4 FragColor;
uniform sampler2D textcrate;

void main()
{
	FragColor = texture(textcrate, vUV);
}