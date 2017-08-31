#include "LoadFile.h"
LoadFile::LoadFile() : m_data(nullptr), m_file(nullptr) {}
LoadFile::~LoadFile() {}
const char* LoadFile::ReadFile(const char* FileName)
{
	fopen_s(&m_file, FileName, "r");
	if (m_file == nullptr)
		perror("no file found");
	fseek(m_file, 0, SEEK_END);
	auto size = ftell(m_file);
	auto buffer = new char[size + 1];
	fseek(m_file, 0, SEEK_SET);
	auto num = fread(buffer, sizeof(char), size + 1, m_file);
	buffer[num] = '\0';
	this->m_data = buffer;
	return this->m_data;
}
