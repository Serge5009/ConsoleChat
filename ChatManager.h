#pragma once
#include <iostream>

#include "User.h"

using namespace std;

class ChatManager
{
public:
	static ChatManager* GetInstance()	//	Singletone
	{
		if (instance == nullptr)
		{
			instance = new ChatManager();
		}
		return instance;
	}

	static const int MAX_CONNECTIONS = 4;	//!!!	Must be same as in NetworkManager
	static const int MAX_USERS = MAX_CONNECTIONS + 1;

	bool isServer;

	User* user[MAX_USERS];

	void GetInput();
	void Update();
	void Render();

	void ConfigProfile();
	string GetYourName() { return user[0]->GetName(); }
	string GetName(int id) { return user[id]->GetName(); }

private:
	static ChatManager* instance;
	ChatManager();
	~ChatManager();

	const string DEFAULT_SERVER_NAME = "SERVER";

public:
};

