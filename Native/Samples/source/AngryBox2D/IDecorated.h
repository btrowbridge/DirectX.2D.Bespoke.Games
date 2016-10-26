#pragma once


class IDecorated
{
public:
	IDecorated(){}
	virtual ~IDecorated() = 0;

	char* GetClass();
	const std::map<char *, void *>* Properties() const;
protected:
	std::map<char *, void*> mProperties;
	char* mClassName;
};

