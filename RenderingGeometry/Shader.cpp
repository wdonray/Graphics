#include "Shader.h"
Shader::Shader(): m_vertexShader(0), m_fragmentShader(0), 
vsSource(nullptr), fsSource(nullptr), m_program(0)
{
}


Shader::~Shader()
{
}

void Shader::bind()
{
}

void Shader::unbind()
{
}

void Shader::attach()
{
}

void Shader::defaultLoad()
{
}

unsigned Shader::getUniform(const char*)
{
	return 0;
}
