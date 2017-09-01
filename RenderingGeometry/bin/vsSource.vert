#version 410
layout(location=0) in vec4 position; 
layout(location=1) in vec4 colour; 
out vec4 vColour; 
uniform mat4 projectionViewWorldMatrix; 
uniform float time;
void main() 
{ 
	vec4 c = vec4(colour.x * sin(time), colour.y * cos(time), colour.z * tan(time), 1);	
	vColour = c;
    gl_Position = projectionViewWorldMatrix * position;
 }