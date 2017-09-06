#pragma once
struct LoadFile;
class Shader
{
public:
	Shader();
	~Shader();
	void bind();
	void unbind();
	void load(const char * filename, unsigned int type);
	void attach();
	void defaultLoad();
	unsigned int getUniform(const char * name);
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	const char* vsSource;
	const char* fsSource;
	LoadFile * fl;

private:
	unsigned int m_program;
};

