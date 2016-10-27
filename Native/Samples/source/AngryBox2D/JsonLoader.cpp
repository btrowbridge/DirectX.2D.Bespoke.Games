#include "pch.h"
#include "JsonLoader.h"
#include <fstream>
#include <string>

using namespace Library;
namespace AngryBox2DGame {

	unique_ptr<JsonLoader> JsonLoader::sInstance;

	void JsonLoader::Initialize()
	{
		sInstance = make_unique<JsonLoader>();
	}

	void JsonLoader::Shutdown()
	{
		sInstance = nullptr;
	}

	void JsonLoader::LoadSpriteDefFromFile(Library::Box2DSpriteDef* spriteDef, wstring filename)
	{
		json spriteDefJson = sInstance->LoadJsonFileContent(filename);
		sInstance->LoadJsonToSpriteDef(spriteDefJson, spriteDef);
	}

	void JsonLoader::WriteSpriteDefToFile(Library::Box2DSpriteDef * spriteDef, wstring filename)
	{
		json jdoc = sInstance->SpriteDefToJson(spriteDef);
		sInstance->WriteJsonToFile(jdoc, filename);
	}

	json JsonLoader::LoadJsonFileContent(wstring filename)
	{
		std::ifstream ifs(filename);
		std::wstring content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		return json(content);
	}

	void JsonLoader::WriteJsonToFile(json jdoc, wstring filename)
	{
		std::string input = jdoc.dump();
		std::ofstream out(filename);
		out << input;
		out.close();
	}


	void JsonLoader::LoadJsonToSpriteDef(json jdoc, Box2DSpriteDef* spriteDef)
	{
		spriteDef->BodyDef.active = jdoc["Box2DSpriteDef"]["BodyDef"]["active"].get<bool>();
		spriteDef->BodyDef.allowSleep = jdoc["Box2DSpriteDef"]["BodyDef"]["allowSleep"].get<bool>();
		spriteDef->BodyDef.angle = jdoc["Box2DSpriteDef"]["BodyDef"]["angle"].get<float32>();
		spriteDef->BodyDef.angularDamping = jdoc["Box2DSpriteDef"]["BodyDef"]["angularDamping"].get<float32>();
		spriteDef->BodyDef.angularVelocity = jdoc["Box2DSpriteDef"]["BodyDef"]["angularVelocity"].get<float32>();
		spriteDef->BodyDef.awake = jdoc["Box2DSpriteDef"]["BodyDef"]["awake"].get<bool>();
		spriteDef->BodyDef.bullet = jdoc["Box2DSpriteDef"]["BodyDef"]["bullet"].get<bool>();
		spriteDef->BodyDef.fixedRotation = jdoc["Box2DSpriteDef"]["BodyDef"]["fixedRotation"].get<bool>();
		spriteDef->BodyDef.gravityScale =  jdoc["Box2DSpriteDef"]["BodyDef"]["gravityScale"].get<float32>();
		spriteDef->BodyDef.linearDamping = jdoc["Box2DSpriteDef"]["BodyDef"]["linearDamping"].get<float32>();
		spriteDef->BodyDef.linearVelocity.x = jdoc["Box2DSpriteDef"]["BodyDef"]["linearVelocity"]["x"].get<float32>();
		spriteDef->BodyDef.linearVelocity.y = jdoc["Box2DSpriteDef"]["BodyDef"]["linearVelocity"]["y"].get<float32>();
		spriteDef->BodyDef.position.x = jdoc["Box2DSpriteDef"]["BodyDef"]["position"]["x"].get<float32>();
		spriteDef->BodyDef.position.y = jdoc["Box2DSpriteDef"]["BodyDef"]["position"]["y"].get<float32>();
		spriteDef->BodyDef.type = static_cast<b2BodyType>(jdoc["Box2DSpriteDef"]["BodyDef"]["bullet"].get<int>());
		spriteDef->BodyDef.userData = make_shared<string>(jdoc["Box2DSpriteDef"]["BodyDef"]["userData"].get<string>()).get();

		spriteDef->Friction = jdoc["Box2DSpriteDef"]["Friction"].get<float>();
		spriteDef->Restitution = jdoc["Box2DSpriteDef"]["Restitution"].get<float>();
		spriteDef->Density = jdoc["Box2DSpriteDef"]["Density"].get<float>();
		spriteDef->IsSensor = jdoc["Box2DSpriteDef"]["IsSensor"].get<bool>();
		spriteDef->Filter.categoryBits = jdoc["Box2DSpriteDef"]["Filter"]["categoryBits"].get<uint16>();
		spriteDef->Filter.maskBits = jdoc["Box2DSpriteDef"]["Filter"]["maskBits"].get<uint16>();
		spriteDef->Filter.groupIndex = jdoc["Box2DSpriteDef"]["Filter"]["maskBits"].get<int16>();

		//Shape defined before loading json from completed sprite definition
		//spriteDef->Shape().m_type = jdoc["Box2DSpriteDef"]["Shape"]["m_type"].get<b2Shape::Type>();
		//spriteDef->Shape().m_radius = jdoc["Box2DSpriteDef"]["Shape"]["m_radius"].get<float32>();
	}

	json JsonLoader::SpriteDefToJson(Library::Box2DSpriteDef* spriteDef)
	{
		json jdoc = { "Box2DSpriteDef" ,
							{ "BodyDef", {
								{"userData", reinterpret_cast<Box2DBehavior*>(spriteDef->BodyDef.userData)->Tag() }, //Use Tag as object identifier
								{"position" , {
									{"x", spriteDef->BodyDef.position.x},
									{"y", spriteDef->BodyDef.position.y}
									}
								},
								{"angle" , spriteDef->BodyDef.angle},
								{"linearVelocity" , {
									{"x", spriteDef->BodyDef.linearVelocity.x},
									{"y", spriteDef->BodyDef.linearVelocity.y}}
								},
								{"angularVelocity" ,spriteDef->BodyDef.angularVelocity},
								{"linearDamping" , spriteDef->BodyDef.linearDamping},
								{"angularDamping" ,spriteDef->BodyDef.angularDamping},
								{"allowSleep" , spriteDef->BodyDef.allowSleep},
								{"awake" , spriteDef->BodyDef.awake},
								{"fixedRotation" , spriteDef->BodyDef.fixedRotation},
								{"bullet" , spriteDef->BodyDef.bullet},
								{"type" , (int)spriteDef->BodyDef.type},
								{"active" , spriteDef->BodyDef.active},
								{"gravityScale" ,spriteDef->BodyDef.gravityScale}
							},
							{"Friction", spriteDef->Friction},
							{"Restitution", spriteDef->Restitution},
							{"Density", spriteDef->Density},
							{"IsSensor", spriteDef->IsSensor},
							{"Filter",{
								{"categoryBits" , spriteDef->Filter.categoryBits},
								{"maskBits" , spriteDef->Filter.maskBits},
								{"groupIndex" , spriteDef->Filter.groupIndex}}
							},
							{"Shape" , {
								{"m_type" , spriteDef->Shape().m_type},
								{"m_radius" , spriteDef->Shape().m_radius}}
							}
						}
					};

		return jdoc;
	}
}

//Sample Json with default
//
//{ "Box2DSpriteDef": {
//		"BodyDef": {
//			"userData": NULL,
//			"position":{
//					X:0.0f,
//					Y:0.0f
//					},
//			"angle": 0.0f,
//			"linearVelocity":{
//					X:0.0f,
//					Y:0.0f
//					},
//			"angularVelocity: 0.0f,
//			"linearDamping" : 0.0f,
//			"angularDamping": 0.0f,
//			"allowSleep": true,
//			"awake": true,
//			"fixedRotation" : false,
//			"bullet" : false
//			"type": (int)b2_staticBody
//			"active": true,
//			"gravityScale" : 1.0f
//			},
//		"Friction: float32,
//		"Restitution" float32,
//		"Density: float32,
//		"IsSensor: false,
//		"Filter":{
//			"categoryBits" : 0x0001,
//			"maskBits" : 0xFFFF,
//			"groupIndex" : 0
//			}/*,
//		"Shape" : {
//			"m_type" : (int) Type,
//			"m_radius" : float32
//		}*/
//}