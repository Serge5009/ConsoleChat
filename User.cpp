#include "User.h"

User::User()
{
	name = "User";
}

User::User(string name)
{
	name = name;
}

User::~User()
{
}

void User::CoutName()
{
	cout << name;
}

