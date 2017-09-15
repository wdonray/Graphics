// // Phong equation
#version 410
layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec4 Normal;
layout (location = 3) in vec2 UV;
layout (location = 4) in vec4 Tangent;

uniform mat4 ProjectionViewModel;
out vec4 vPosition;
out vec4 vColor;
out vec4 vNormal;
out vec2 vUV;
out vec4 vTangent;

void main()
{
	vPosition = Position;
	vColor = Color;
	vNormal = Normal;
	vUV = UV;
	vTangent = Tangent;
	gl_Position = ProjectionViewModel * vPosition;
}