#include "pch.h"
#include "JsonLoader.h"
#include <fstream>


b2BodyDef JsonLoader::LoadBodyDef(wstring filename)
{
	wstring fileContents = LoadFileContentToString(filename);

	json bodyDefJson(fileContents);

	b2BodyDef bodyDef;
}

wstring JsonLoader::LoadFileContentToString(wstring filename)
{
	std::ifstream ifs(filename);
	std::wstring content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return content;
}
