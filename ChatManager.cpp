#include "ChatManager.h"
#include <string>

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
				if (isServer)
				{
					chat.append(ChatManager::GetInstance()->GetName(0));
					chat.append("\n");
					chat.append(message);
					chat.append("\n\n");
				}
				SendTextMessage(message);
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
				ReceiveMessage(rcvMessage);

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
				ReceiveMessage(rcvMessage);
			}
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

void ChatManager::ReceiveMessage(char* data)
{
	if (isServer)
	{
		NetworkManager::GetInstance()->SendDataTCP(data);	//	Server forwards the message before processing it
	}

	DataToSend* receivedData = new DataToSend;
	receivedData = reinterpret_cast<DataToSend*>(data);

	chat.append(receivedData->userName);
	chat.append(":\n");
	chat.append(receivedData->message);
	chat.append("\n\n");

	//delete receivedData;
}

void ChatManager::SendTextMessage(string messageToSend)
{
	DataToSend* data = new DataToSend;
	data->userName = user[0]->GetName();
	//data->userColor =		//	prefered color
	//data->userMessageColor =		//	prefered color
	data->message = messageToSend;

	char* messageBuffer = new char[sizeof(data)];
	messageBuffer = reinterpret_cast<char*>(data);

	NetworkManager::GetInstance()->SendDataTCP(messageBuffer);

	//delete data;
	//delete[] messageBuffer;
}

ChatManager::~ChatManager()
{
	for (int i = 0; i < MAX_USERS; i++)
	{
		delete user[i];
		user[i] = nullptr;
	}
}
