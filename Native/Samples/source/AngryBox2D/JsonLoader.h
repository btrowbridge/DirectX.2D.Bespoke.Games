#pragma once
#include <Box2D\Box2D.h>
#include "json.hpp"
#include <iostream>
#include "ReflectionHelper.h"

using json = nlohmann::json;
using namespace std;
using namespace Reflection;

static class JsonLoader
{
public:
	
	template <struct T>
	T LoadStruct(wstring filename);

	template <struct T>
	void WriteStruct(T object, wstring filename);

	b2BodyDef LoadBodyDef(wstring filename);
	void WriteBodyDef(wstring filename);

private:
	wstring LoadFileContentToString(wstring filename);

	template <struct T>
	wstring Serialize(T object);

	template <struct T>
	T Deserialize(wstring objectString);
};



namespace ReflectableTypes{

	template<struct Parent, struct Child>
	inline Parent* Deflect(Child c) {
		return reinterpret_cast<Parent>(c);
	}

	template<struct Parent, struct Child>
	inline Child* Reflect(Parent p) {
		return reinterpret_cast<Child>(p);
	}


	struct rb2BodyDef : b2BodyDef
	{
		rb2BodyDef(b2BodyDef parent) : b2BodyDef(parent)
		{	
		}
	
		REFLECTABLE(
		(bool) active,
		(bool) allowSleep,
		(float32) angle,
		(float32) angularDamping,
		(float32) angularVelocity,
		(bool) awake,
		(bool) bullet,
		(bool) fixedRotation,
		(b2Vec2) position,
		(b2BodyType) type,
		(void*) userData
		)
	};

};

