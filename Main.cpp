#include <iostream>
#include "NetworkManager.h"
#include "ChatManager.h"
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

	ChatManager::GetInstance();

	if (userInpurRole == SERVER)	//	Initializing ChatManager and setting isServer
	{
		ChatManager::GetInstance()->isServer = true;
	}
	else
	{
		ChatManager::GetInstance()->isServer = false;
	}
	ChatManager::GetInstance()->ConfigProfile();	//	We configure only client profile



	if (ChatManager::GetInstance()->isServer)
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



	//	User message
	string message;
	message[0] = '\0';

	string chat;	// This stores all previouse messages to be rendered each frame
	chat[0] = '\0';

	int errorCount = 0;
	char rcvMessage[NetworkManager::MAX_MSG_SIZE];

	while (errorCount == 0)
	{
		errorCount += ChatManager::GetInstance()->GetInput();
		errorCount += ChatManager::GetInstance()->Update();
		ChatManager::GetInstance()->Render();




		//	Server connection accepting
		if (ChatManager::GetInstance()->isServer)	//	Server tries to accept conection
		{
			for (int i = NetworkManager::GetInstance()->GetNumConnections(); i < NetworkManager::MAX_CONNECTIONS; i++)
			{
				NetworkManager::GetInstance()->AcceptConnectionTCP(i);
			}
		}


		

		Sleep(30);
	}
	NetworkManager::GetInstance()->Shutdown();

	return 0;
}
