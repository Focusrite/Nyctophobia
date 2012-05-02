#include "FileInput.h"

using namespace std;

std::string FileInput::getFileAsString(const char* path)
{
	ifstream fin(path);
	if (fin.fail()) return "ERROR: FILE NOT FOUND";

	fin.seekg(0, ios::end);
	size_t length = fin.tellg();
	fin.seekg(0, ios::beg);
	char* buffer = new char[length + 1];
	fin.read(buffer, length);
	buffer[length] = '\0';

	fin.close();
	string str(buffer);
	delete buffer;

	return str;
}