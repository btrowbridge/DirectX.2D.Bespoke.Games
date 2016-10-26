#include "pch.h"
#include "JsonLoader.h"
#include <fstream>


json JsonLoader::LoadJsonFileContent(wstring filename)
{
	std::ifstream ifs(filename);
	std::wstring content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return json(content);
}
