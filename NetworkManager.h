#pragma once
#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;

class NetworkManager
{
public:

	static NetworkManager* GetInstance()	//	Singletone
	{
		if (instance == nullptr)
		{
			instance = new NetworkManager();
		}
		return instance;
	}

	void Init();
	void Shutdown();
	void CreateUDPSockets();
	void CreateTCPSockets();
	void BindUDP();


		//	SERVER:
	void BindTCP(int port);
	void ListenTCP();
	void AcceptConnectionTCP(int clientID);

		//	CLIENT:
	void ConnectTCP(int port, string ip);

		//	BOTH
	void SendDataTCP(const char* data);
	int ReceiveDataTCP(char* ReceiveBuffer, int connectionID);

	void SetRemoteData();	
	void SendData(const char* data);
	int ReceiveData(char* ReceiveBuffer);
	int GetNumConnections() { return numConnections; }

	
	static const int MAX_MSG_SIZE = 1500;
	static const int MAX_CONNECTIONS = 2;

private:

	NetworkManager();
	~NetworkManager();

	SOCKET TCPSocketIn;
	SOCKET TCPSocketOut[MAX_CONNECTIONS];


	SOCKADDR_IN TCPinAddr;
	SOCKADDR_IN TCPoutAddr[MAX_CONNECTIONS];

	int numConnections = 0;

	static NetworkManager* instance;







	SOCKET UDPSocketIn;
	SOCKET UDPSocketOut;
	SOCKADDR_IN UDPoutAddr;
	SOCKADDR_IN UDPinAddr;
};
