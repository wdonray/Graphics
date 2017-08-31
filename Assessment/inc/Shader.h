#pragma once
#include <cstdio>
class Shader
{
public:
	Shader();
	~Shader();

	const char * load(const char *);
private:
	const char * m_data;
	FILE* m_file;
};

