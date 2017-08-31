#version 410
layout(location=0) in vec4 position; 
layout(location=1) in vec4 colour; 
out vec4 vColour; 
uniform mat4 projectionViewWorldMatrix; 
uniform float time;
void main() 
{ 
vColour = colour;
vec4 c = vec4(1,abs(tan(time)),0,0);
vColour = vColour + c;
gl_Position = projectionViewWorldMatrix * position;
 }