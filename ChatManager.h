#pragma once
#include <iostream>
#include <conio.h>

#include "NetworkManager.h"
#include "DataToSend.h"

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

	User* user[MAX_USERS];	//	Now only 1 user is used by each instance of a programm
	int numUsers;

	int GetInput();
	int Update();
	void Render();

	void ConfigProfile();
	string GetName(int id) { return user[id]->GetName(); }

	void ReceiveMessage(char* data);
	void SendTextMessage(string messageToSend);

	string colorCode[NUM_COLOR];

private:
	static ChatManager* instance;
	ChatManager();
	~ChatManager();


	string message;
	string chat;

	char rcvMessage[NetworkManager::MAX_MSG_SIZE];

	const string DEFAULT_SERVER_NAME = "SERVER";

};

