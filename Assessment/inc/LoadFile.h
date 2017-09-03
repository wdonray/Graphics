#pragma once
#include <cstdio>
class LoadFile
{
public:
	LoadFile();
	~LoadFile();

	const char * load(const char *);
private:
	const char * m_data;
	FILE* m_file;
};

