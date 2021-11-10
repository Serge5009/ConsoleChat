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

private:
	static ChatManager* instance;
	ChatManager();
	~ChatManager();


	User user[MAX_USERS];


public:
};

