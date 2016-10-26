#include "pch.h"
#include "IDecorated.h"



char * IDecorated::GetClass()
{
	return mClassName;
}

const std::map<char*, void*>* IDecorated::Properties() const
{
	return &mProperties;
}
