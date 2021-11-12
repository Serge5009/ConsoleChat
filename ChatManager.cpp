#include "ChatManager.h"

ChatManager* ChatManager::instance = nullptr;

ChatManager::ChatManager()
{

	//	Setting defaults
	isServer = false;
	message[0] = '\0';
	chat[0] = '\0';
	rcvMessage[0] = '\0';
	numUsers = 0;

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
				chat.append(ChatManager::GetInstance()->GetYourName());
				chat.append(":\n");
				chat.append(message);
				chat.append("\n\n");

				message.insert(0, "M");
				NetworkManager::GetInstance()->SendDataTCP(message.c_str());
			}


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

	if (isServer)
	{
		for (int i = 0; i < NetworkManager::GetInstance()->GetNumConnections(); i++)
		{
			int size = NetworkManager::GetInstance()->ReceiveDataTCP(rcvMessage, i);

			if (size > 0)
			{
				if (rcvMessage[0] == 'P')
				{
					AddNewUser(rcvMessage);
				}
				if (rcvMessage[0] == 'M')
				{
					ReceiveMessage(rcvMessage, i);
				}

			}
		}

		//	Server tries to accept connections in background	(he's doing his best o.O)
		for (int i = NetworkManager::GetInstance()->GetNumConnections(); i < NetworkManager::MAX_CONNECTIONS; i++)
		{
			NetworkManager::GetInstance()->AcceptConnectionTCP(i);
		}

	}
	else
	{
		for (int i = 0; i < NetworkManager::GetInstance()->GetNumConnections(); i++)
		{
			int size = NetworkManager::GetInstance()->ReceiveDataTCP(rcvMessage, i);

			if (size > 0)
			{
				ReceiveMessage(rcvMessage, i);
			}
		}
	}





	

	//	Server connection accepting
	


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

void ChatManager::AddNewUser(char* data)
{
	string userData;
	userData[0] = '\0';
	userData.append(data);

	userData.erase(0, 1);
 	cout << userData;

	for (int i = 0; i < MAX_USERS; i++)
	{
		if (user[i] == nullptr)
		{
			user[i] = new User;
			user[i]->SetName(userData);
			user[i]->socketId = i - 1;

			numUsers++;
			return;
		}
	}

}

void ChatManager::ReceiveMessage(char* data, int id)
{
	string messageData;
	messageData[0] = '\0';
	messageData.append(data);

	messageData.erase(0, 1);

	string name = "Unknown";
	for (int i = 0; i <= numUsers; i++)
	{
		if(user[i]->socketId == id)
			chat.append(user[i]->GetName());
	}
	chat.append(":\n");

	chat.append(messageData);
	chat.append("\n\n");



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
