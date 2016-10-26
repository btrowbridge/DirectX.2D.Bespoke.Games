#include "pch.h"
#include "JsonLoader.h"
#include <fstream>


Library::Box2DSpriteDef JsonLoader::LoadSpriteDefFromFile(wstring filename)
{
	
}

void JsonLoader::WriteSpriteDefToFile(Library::Box2DSpriteDef * spriteDef, wstring filename)
{
}

json JsonLoader::LoadJsonFileContent(wstring filename)
{
	std::ifstream ifs(filename);
	std::wstring content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return json(content);
}

Library::Box2DSpriteDef & JsonLoader::JSONtoSpriteDef(json jdoc)
{
	// TODO: insert return statement here
}

json JsonLoader::SpriteDefToJson(Library::Box2DSpriteDef & spriteDef)
{
	return json();
}
