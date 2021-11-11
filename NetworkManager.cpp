#include "NetworkManager.h"
#include <iostream>
#include <WS2tcpip.h>

using namespace std;

NetworkManager* NetworkManager::instance = nullptr;

NetworkManager::NetworkManager()
{
	//	Setting defaults on initialization
	UDPSocketIn = INVALID_SOCKET;
	UDPSocketOut = INVALID_SOCKET;

	UDPoutAddr = { 0 };
	UDPinAddr = { 0 };

	TCPSocketIn = INVALID_SOCKET;
	TCPinAddr = { 0 };


	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		TCPSocketOut[i] = INVALID_SOCKET;

		TCPoutAddr[i] = { 0 };
	}

}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Init()
{
	cout << "NetworkManager::Init()" << endl;

	WSADATA LpWSAData;
	int error = WSAStartup(MAKEWORD(2, 2), &LpWSAData);

	if (error != 0)					
	{	//error message
		cout << "WSAData faied with error code " << error << endl;
	}
}

void  NetworkManager::CreateUDPSockets()
{
	UDPSocketIn = socket(AF_INET, SOCK_DGRAM, 0);

	if (UDPSocketIn == INVALID_SOCKET)
	{	//error message
		cout << "Failed to create the inbound socket" << endl;
		Shutdown();
	}

	UDPSocketOut = socket(AF_INET, SOCK_DGRAM, 0);

	if (UDPSocketOut == INVALID_SOCKET)
	{	//error message
		cout << "Failed to create the Outbound socket" << endl;
		Shutdown();
	}
}

void  NetworkManager::CreateTCPSockets()
{
	TCPSocketIn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (TCPSocketIn == INVALID_SOCKET)
	{	//error message
		cout << "Failed to create the inbound socket" << endl;
		Shutdown();
	}

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		
		TCPSocketOut[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (TCPSocketOut[i] == INVALID_SOCKET)
		{	//error message
			cout << "Failed to create the Outbound socket" << endl;
			Shutdown();
		}
	}

}


void NetworkManager::BindTCP(int port)	//	Server
{
	port = 7777;


	TCPinAddr.sin_family = AF_INET;
	TCPinAddr.sin_port = htons(port);
	TCPinAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = bind(TCPSocketIn, reinterpret_cast<SOCKADDR*>(&TCPinAddr), sizeof(TCPinAddr));

	if (bindError == SOCKET_ERROR)
	{	//error message
		cout << "failed to bind the inbound TCP sockets" << endl;
		Shutdown();
	}

}

void NetworkManager::ListenTCP()	//	Server
{
	listen(TCPSocketIn, SOMAXCONN);
}

void NetworkManager::ConnectTCP(int port, string ip)	//	Client
{
	port = 7777;
	ip = "127.0.0.1";
	
	TCPoutAddr[0].sin_family = AF_INET;
	TCPoutAddr[0].sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &TCPoutAddr[0].sin_addr);

	int connectStatus = connect(TCPSocketOut[0], reinterpret_cast<sockaddr*>(&TCPoutAddr[0]), sizeof(TCPoutAddr[0]));

	if (connectStatus == SOCKET_ERROR)
	{	//error message
		cout << "Error, failed to connecting through tcp socket info supplied!" << endl;
		Shutdown();
	}

	numConnections++;
	unsigned long bit = 1;
	int results = ioctlsocket(TCPSocketOut[0], FIONBIO, &bit);
	if (results != NO_ERROR)
	{	//error message
		cout << "error for ioctlsocket TCPsocket out" << endl;
	}

}

void NetworkManager::AcceptConnectionTCP(int clientID)	//	Server
{
	int clientSize = sizeof(TCPoutAddr);

	TCPSocketOut[clientID] = accept(TCPSocketIn, reinterpret_cast<SOCKADDR*>(&TCPoutAddr), &clientSize);

	if (TCPSocketOut[clientID] != INVALID_SOCKET)
	{
		char ipConnected[32];

		inet_ntop(AF_INET, &TCPoutAddr[clientID].sin_addr, ipConnected, sizeof(ipConnected));

		cout << ipConnected << " - Connected!" << endl;
		numConnections++;
	}


	unsigned long bit2 = 1;
	int results = ioctlsocket(TCPSocketOut[clientID], FIONBIO, &bit2);
	if (results != NO_ERROR)
	{	//error message
		cout << "error for ioctlsocket TCPsocket out" << endl;
	}


	unsigned long bit = 1;
	results = ioctlsocket(TCPSocketIn, FIONBIO, &bit);
	if (results != NO_ERROR)
	{	//error message
		cout << "error for ioctlsocket TCPsocket in" << endl;
	}
}


void NetworkManager::SendDataTCP(const char* data)		//	If client disconects server will crash on data sending
{
	for (int i = 0; i < numConnections; i++)	
	{
		int totalByteSent = send(TCPSocketOut[i], data, MAX_MSG_SIZE, 0);

		if (totalByteSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
			{
				// Not a problem, keep going
				cout << "Sent the data across: " << data << "\nsize was: " << totalByteSent << endl;
			}
			else
			{	//error message
				cout << "Error, TCP failed to send message" << endl;
				Shutdown();
			}
		}
	}
}


int NetworkManager::ReceiveDataTCP(char* ReceiveBuffer, int connectionID)
{

	int BytesReceived = 0;
	BytesReceived = recv(TCPSocketOut[connectionID], ReceiveBuffer, MAX_MSG_SIZE, 0);

	if (BytesReceived == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{	//error message
			cout << "Error TCP failed to receive a message" << endl;
			Shutdown();
		}
	}
	return BytesReceived;
}

void NetworkManager::Shutdown()
{
	cout << "NetworkManager::Shutdown()" << endl;

	int errorCode = WSAGetLastError();
	if (errorCode != 0)
	{	//error message
		cout << "Forced shutdown due to WSAError" << errorCode << endl;
	}

	if (UDPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{	//error message
			cout << "[ERROR] closing the UDP in socket" << endl;
		}
	}

	if (UDPSocketOut != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketOut) != 0)
		{	//error message
			cout << "[ERROR] closing the UDP out socket" << endl;
		}
	}

	if (TCPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(TCPSocketIn) != 0)
		{	//error message
			cout << "[ERROR] closing the TCP in socket" << endl;
		}
	}

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (TCPSocketOut[i] != INVALID_SOCKET)
		{
			if (closesocket(TCPSocketOut[i]) != 0)
			{	//error message
				cout << "[ERROR] closing the TCP out socket" << endl;
			}
		}
	}

	
	WSACleanup();
	exit(0);
}


/// 
///			UDP part (OLD)
/// 

void NetworkManager::BindUDP()
{
	UDPinAddr.sin_family = AF_INET;
	UDPinAddr.sin_port = htons(8889);
	UDPinAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = bind(UDPSocketIn, reinterpret_cast<SOCKADDR*>(&UDPinAddr), sizeof(UDPinAddr));

	if (bindError == SOCKET_ERROR)
	{	//error message
		cout << "failed to bind the inbound UDP sockets!" << endl;
		Shutdown();
	}
}


void NetworkManager::SetRemoteData()
{
	UDPoutAddr.sin_family = AF_INET;
	UDPoutAddr.sin_port = htons(8889);
	inet_pton(AF_INET, "127.0.0.1", &UDPoutAddr.sin_addr);

}

void NetworkManager::SendData(const char* data)
{
	int totalByteSent = sendto(UDPSocketOut, data, MAX_MSG_SIZE, 0,
		reinterpret_cast<SOCKADDR*>(&UDPoutAddr), sizeof(UDPoutAddr));

	cout << totalByteSent << " bytes of data has been sent!" << endl;

	if (totalByteSent == SOCKET_ERROR)
	{
		Shutdown();
	}
}

int NetworkManager::ReceiveData(char* ReceiveBuffer)
{
	int BytesReceived = 0;
	int inAddrSize = sizeof(UDPinAddr);

	BytesReceived = recvfrom(UDPSocketIn, ReceiveBuffer, MAX_MSG_SIZE, 0,
		reinterpret_cast<SOCKADDR*>(&UDPinAddr), &inAddrSize);

	if (BytesReceived == SOCKET_ERROR)
	{
		Shutdown();
	}
	return BytesReceived;
}
