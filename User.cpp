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

void User::ConfigureUser()
{
	cout << "Please, enter your name: " << endl;
	cin >> name;
	//	Color change here later



}

void User::CoutName()
{
	cout << name;
}

