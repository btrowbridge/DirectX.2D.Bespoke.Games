#pragma once
#include <Box2D\Box2D.h>
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;
using namespace std;

class JsonLoader
{
public:

private:
	json LoadJsonFileContent(wstring filename);
};