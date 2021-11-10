#include <iostream>
#include "NetworkManager.h"
#include <string>
#include <conio.h>


using namespace std;

int main()
{
	NetworkManager::GetInstance()->Init();

	NetworkManager::GetInstance()->CreateTCPSockets();

	const int SERVER = 1;	

	cout << "Please you are a " << endl;
	cout << "\tServer\t[" << SERVER << "]" << endl;
	cout << "\tClient\t[2]" << endl;

	int userInpurRole = 0;
	cin >> userInpurRole;

	if (userInpurRole == SERVER)
	{
		int serverPort;
		cout << "Please, enter your port: " << endl;
		cin >> serverPort;

		NetworkManager::GetInstance()->BindTCP(serverPort);
		NetworkManager::GetInstance()->ListenTCP();
	}
	else
	{
		int clientPort;
		cout << "Please, enter your destination port: " << endl;
		cin >> clientPort;

		char* serverIp = new char[32];
		cout << "Please, enter your destination IP: " << endl;
		cin >> serverIp;

		NetworkManager::GetInstance()->ConnectTCP(clientPort, serverIp);
	}

	/*
	while (NetworkManager::GetInstance()->GetNumConnections() <= 0)	//	going to endless loop until connect
	{
		if (userInpurRole == SERVER)	//	Server tries to accept conection
		{
			NetworkManager::GetInstance()->AcceptConnectionTCP();
		}
	}*/

	//	User message
	string message;
	message[0] = '\0';

	string chat;	// This stores all previouse messages to be rendered each frame
	chat[0] = '\0';



	bool isRunning = true;
	char rcvMessage[NetworkManager::MAX_MSG_SIZE];

	while (isRunning)
	{
		//	Server connection accepting
		if (userInpurRole == SERVER)	//	Server tries to accept conection
		{
			for (int i = NetworkManager::GetInstance()->GetNumConnections(); i < NetworkManager::MAX_CONNECTIONS; i++)
			{
				NetworkManager::GetInstance()->AcceptConnectionTCP(i);
			}
		}




		//	UI and Message sending
		system("CLS");	//	Cleare screen at the beginnign of the frame
		if (userInpurRole == SERVER)
		{
			cout << "SERVER" << "\tConnections: "<< NetworkManager::GetInstance()->GetNumConnections() << endl;
		}
		else
		{
			cout << "CLIENT" << endl;
		}
		



		cout << chat << endl;
		cout << "Send: " << message << endl;


		//	User input
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
				chat.append("you:\t\t");
				chat.append(message);
				chat.append("\n\n");

				//	clear our current input
				message.erase();
			}
			if (int(key) == 27)	//	ESC key
			{
				isRunning = false;
			}
			if (int(key) == 8)	//	BACKSPACE key
			{
				message = message.substr(0, message.size() - 2);
			}
		}

		//	Message receiving

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

		Sleep(30);
	}
	NetworkManager::GetInstance()->Shutdown();

	return 0;
}
