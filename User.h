#pragma once
#include <iostream>

using namespace std;

enum Color
{
	RED,
	GREEN,
	BLUE,

	NUM_COLOR
};

class User
{
private:
	string name;
	//Color color;

public:

	User();
	User(string name);
	~User();

	void SetName(string Name) { name = Name; }
	string GetName() { return name; }

	
	void CoutName();

};

