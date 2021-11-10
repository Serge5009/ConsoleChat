#include "ChatManager.h"

ChatManager* ChatManager::instance = nullptr;

ChatManager::ChatManager()
{

	//	Setting defaults
	isServer = false;

	for (int i = 0; i < MAX_USERS; i++)
	{
		user[i] = nullptr;
	}
}


void ChatManager::GetInput()
{
}

void ChatManager::Update()
{
}

void ChatManager::Render()
{
}

void ChatManager::ConfigProfile()
{
	if (!isServer)
	{
		if (user[0] == nullptr)
			user[0] = new User;
		user[0]->ConfigureUser();
	}
	else
	{
		if (user[0] == nullptr)
			user[0] = new User;
		user[0]->SetName(DEFAULT_SERVER_NAME);
	}
}


ChatManager::~ChatManager()
{
	for (int i = 0; i < MAX_USERS; i++)
	{
		delete user[i];
		user[i] = nullptr;
	}
}
