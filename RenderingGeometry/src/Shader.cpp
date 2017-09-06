#include "Shader.h"
#include <gl_core_4_4.h>
#include <iostream>
#include <LoadFile.h>
Shader::Shader(): m_vertexShader(0), m_fragmentShader(0),
                  vsSource(nullptr), fsSource(nullptr), fl(nullptr), m_program(0)
{
	fl = new LoadFile();
}

Shader::~Shader()
{
	delete fl;
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::unbind()
{
	glUseProgram(0);
}


void Shader::attach()
{
	int success = GL_FALSE;
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, nullptr, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
}

void Shader::defaultLoad()
{
}

unsigned Shader::getUniform(const char* name)
{
	return glGetUniformLocation(m_program, name);
}

void Shader::load(const char * filename, unsigned int type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		vsSource = fl->load(filename);
		m_vertexShader = glCreateShader(type);
		glShaderSource(m_vertexShader, 1, (const char**)&vsSource, nullptr);
		glCompileShader(m_vertexShader);
		break;
	case GL_FRAGMENT_SHADER:
		fsSource = fl->load(filename);
		m_fragmentShader = glCreateShader(type);
		glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, nullptr);
		glCompileShader(m_fragmentShader);
		break;
	default:
		printf("You should not be loading this");
		break;
	}
}