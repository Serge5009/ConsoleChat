#pragma once
#include <iostream>
#include <conio.h>

#include "NetworkManager.h"

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

	int GetInput();
	int Update();
	void Render();

	void ConfigProfile();
	string GetYourName() { return user[0]->GetName(); }
	string GetName(int id) { return user[id]->GetName(); }

	void SendYourProfile();

private:
	static ChatManager* instance;
	ChatManager();
	~ChatManager();

	const string DEFAULT_SERVER_NAME = "SERVER";

	string message;
	string chat;

	char rcvMessage[NetworkManager::MAX_MSG_SIZE];


public:
};

