#pragma once
#include <Box2D\Box2D.h>
#include "json.hpp"
#include <iostream>
#include "Box2DSprite.h"

using json = nlohmann::json;
using namespace std;

namespace Library {
	class Box2DSpriteDef;
}

static class JsonLoader
{
public:
	
	static Library::Box2DSpriteDef& LoadSpriteDefFromFile(wstring filename);

	static void WriteSpriteDefToFile(Library::Box2DSpriteDef* spriteDef, wstring filename);

private:
	json LoadJsonFileContent(wstring filename);

	Library::Box2DSpriteDef& JSONtoSpriteDef(json jdoc);

	json SpriteDefToJson(Library::Box2DSpriteDef& spriteDef);
};