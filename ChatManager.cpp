#include "ChatManager.h"

ChatManager* ChatManager::instance = nullptr;

ChatManager::ChatManager()
{

	//	Setting defaults
	isServer = false;
	message[0] = '\0';
	chat[0] = '\0';
	rcvMessage[0] = '\0';

	for (int i = 0; i < MAX_USERS; i++)
	{
		user[i] = nullptr;
	}
}


int ChatManager::GetInput()
{
	int errorsN = 0;
	if (_kbhit())
	{
		char key = _getch();
		message += key;


		//	Special key handling (should be with switch)
		if (int(key) == 13)	//	ENTER key
		{
			//	Send the message
			if (message.length() > 0)
			{
				NetworkManager::GetInstance()->SendDataTCP(message.c_str());
			}

			//	save what you just sent
			chat.append(ChatManager::GetInstance()->GetYourName());
			chat.append("\t\t");
			chat.append(message);
			chat.append("\n\n");

			//	clear our current input
			message.erase();
		}
		if (int(key) == 27)	//	ESC key
		{
			errorsN++;
		}
		if (int(key) == 8)	//	BACKSPACE key
		{
			message = message.substr(0, message.size() - 2);
		}
	}
	return errorsN;
}

int ChatManager::Update()
{
	int errorsN = 0;


	for (int i = 0; i < NetworkManager::GetInstance()->GetNumConnections(); i++)
	{
		int size = NetworkManager::GetInstance()->ReceiveDataTCP(rcvMessage, i);

		if (size > 0)
		{
			chat.append("received:\t");
			chat.append(rcvMessage);
			chat.append("\n\n");
		}
	}

	return errorsN;
}

void ChatManager::Render()
{
	system("CLS");	//	Cleare screen at the beginnign of the frame
	if (isServer)
	{
		cout << "SERVER" << "\tConnections: " << NetworkManager::GetInstance()->GetNumConnections() << endl;
	}
	else
	{
		cout << "CLIENT" << endl;
	}

	cout << chat << endl;
	cout << "Send: " << message << endl;
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

void ChatManager::SendYourProfile()
{
	string profileToSend = "P";

	profileToSend.append(user[0]->GetName());


	NetworkManager::GetInstance()->SendDataTCP(profileToSend.c_str());
}


ChatManager::~ChatManager()
{
	for (int i = 0; i < MAX_USERS; i++)
	{
		delete user[i];
		user[i] = nullptr;
	}
}
