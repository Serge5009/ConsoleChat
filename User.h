#pragma once
#include <iostream>

using namespace std;

enum Color
{

	RED,
	GREEN,
	BLUE,
	YELLOW,
	PURPLE,
	PINK,
	CYAN,
	GRAY,
	BLACK,
	WHITE,

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

	void ConfigureUser();

	void SetName(string Name) { name = Name; }
	string GetName() { return name; }

	int socketId;
	
	void CoutName();

};

