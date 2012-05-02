#pragma once
#include <fstream>
#include <iostream>
#include <string>

class FileInput
{
public:
	std::string getFileAsString(const char* path);
private:
	
};

extern FileInput* gFileInput;