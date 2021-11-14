#include <iostream>
#include "NetworkManager.h"
#include "ChatManager.h"
#include <string>
#include <conio.h>

//	This is the TCP chat app
//	
//	Known issues:
//	
// 		-Disconected users cause server crash
//		-Server crash causes all clients crash
//		-Longer messages cause server to crash
// 
//	Notes:
// 
//		-All user-inputed ports & IPs are hardcoded to change back to default
//		it is made to save some time on testing
//
// 
//	Assignment report:
// 
//	Majors:
//	1 - Done
//	2 - Done
// 
//	Minors:
// 
//	1 - Colors
//		done for 2 types (message & username colors), displayed to all clients and server properly
// 
//	3 - Message forwarding to all clients 
//		works good, but all profile info with colors is sent with each message that can affect performance 
// 
//	4 - Object serializtion
//		done, we send each message as an object that contains username, colors and message content
//		(in future some optimization can help to move DataToSend object functionality to User object)
// 
//

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
	ChatManager::GetInstance()->ConfigProfile();	



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

		Sleep(30);
	}

	NetworkManager::GetInstance()->Shutdown();

	return 0;
}
