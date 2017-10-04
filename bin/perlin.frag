#version 410 
in vec4 vNormal;
in vec4 vPosition;
in vec4 vColor;
in vec2 vUV;
in vec4 vTangent;
uniform sampler2D perlinTexture; 
out vec4 FragColor;
void main() 
{
	FragColor = texture(perlinTexture, vUV).rrrr; 
	FragColor.a = 1;
}