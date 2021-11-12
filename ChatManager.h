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

	static const int MAX_CONNECTIONS = 2;	//	Defines maximum clients number
	static const int MAX_USERS = MAX_CONNECTIONS + 1;

	bool isServer;

	User* user[MAX_USERS];
	int numUsers;

	int GetInput();
	int Update();
	void Render();

	void ConfigProfile();
	string GetYourName() { return user[0]->GetName(); }
	string GetName(int id) { return user[id]->GetName(); }

	void AddNewUser(char* data);
	void ReceiveMessage(char* data, int id);

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

