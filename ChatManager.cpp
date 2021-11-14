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

	colorCode[RED] = "\033[31m";
	colorCode[GREEN] = "\033[32m";
	colorCode[BLUE] = "\033[34m";
	colorCode[YELLOW] = "\033[33m";
	colorCode[PURPLE] = "\033[35m";
	colorCode[PINK] = "\033[95m";
	colorCode[CYAN] = "\033[36m";
	colorCode[GRAY] = "\033[90m";

	colorCode[BLACK] = "\033[30m";
	colorCode[WHITE] = "\033[37m";


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

					chat.append(colorCode[user[0]->color]);		//	Swithcing to new color
					chat.append(ChatManager::GetInstance()->GetName(0));
					chat.append("\n");
					chat.append(colorCode[WHITE]);				//	Swithcing back to white


					chat.append(colorCode[user[0]->msgColor]);	//	Swithcing to new color
					chat.append(message);
					chat.append("\n\n");
					chat.append(colorCode[WHITE]);				//	Swithcing back to white


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
		cout << colorCode[RED] << "SERVER" << colorCode[WHITE] << "\tConnections: " << NetworkManager::GetInstance()->GetNumConnections() << endl;
	}
	else
	{
		cout << "CLIENT - " << colorCode[user[0]->color] << user[0]->GetName() << colorCode[WHITE] << endl;
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
		user[0]->color = PURPLE;
		user[0]->msgColor = RED;
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

	chat.append(colorCode[receivedData->userColor]);		//	Swithcing to new color
	chat.append(receivedData->userName);
	chat.append(":\n");
	chat.append(colorCode[WHITE]);							//	Swithcing back to white

	chat.append(colorCode[receivedData->userMessageColor]);	//	Swithcing to new color
	chat.append(receivedData->message);
	chat.append("\n\n");
	chat.append(colorCode[WHITE]);							//	Swithcing back to white

	//delete receivedData;
}

void ChatManager::SendTextMessage(string messageToSend)
{
	DataToSend* data = new DataToSend;
	data->userName = user[0]->GetName();
	data->userColor = user[0]->color;
	data->userMessageColor = user[0]->msgColor;
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
