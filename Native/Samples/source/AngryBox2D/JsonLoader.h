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

namespace AngryBox2DGame {
	class JsonLoader
	{
	public:

		static void Initialize();

		static void Shutdown();

		static void LoadSpriteDefFromFile(Library::Box2DSpriteDef* spriteDef, wstring filename);

		static void WriteSpriteDefToFile(Library::Box2DSpriteDef* spriteDef, wstring filename);

		static std::vector<Library::Box2DSpriteDef*> LoadLevelObjectsFromFile(wstring filename);
	private:
		json LoadJsonFileContent(wstring filename);
		void WriteJsonToFile(json jdoc, wstring filename);

		void LoadJsonToSpriteDef(json jdoc, Library::Box2DSpriteDef* spriteDef);

		json SpriteDefToJson(Library::Box2DSpriteDef* spriteDef);

		static unique_ptr<JsonLoader> sInstance;
	};
}