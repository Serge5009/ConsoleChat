#include "User.h"

User::User()
{	//	Setting defaults
	name = "User";
	color = WHITE;
	msgColor = WHITE;
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

	system("CLS");
	cout << "Choose the color for your name: " << endl;
	cout << "1. Red" << endl;
	cout << "2. Green" << endl;
	cout << "3. Blue" << endl;
	cout << "4. Yellow" << endl;
	cout << "5. Purple" << endl;
	cout << "6. Pink" << endl;
	cout << "7. Cyan" << endl;
	cout << "8. Gray" << endl;
	cout << "9. Black" << endl;
	cout << "10. White" << endl;

	int userColorInput = 10;

	cin >> userColorInput;
	userColorInput--;

	color = static_cast<Color>(userColorInput);

	system("CLS");
	cout << "Choose the color for your messages: " << endl;
	cout << "1. Red" << endl;
	cout << "2. Green" << endl;
	cout << "3. Blue" << endl;
	cout << "4. Yellow" << endl;
	cout << "5. Purple" << endl;
	cout << "6. Pink" << endl;
	cout << "7. Cyan" << endl;
	cout << "8. Gray" << endl;
	cout << "9. Black" << endl;
	cout << "10. White" << endl;

	userColorInput = 10;

	cin >> userColorInput;
	userColorInput--;

	msgColor = static_cast<Color>(userColorInput);

}

void User::CoutName()
{
	cout << name;
}

