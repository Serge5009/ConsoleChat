#pragma once

#include "NetworkManager.h"
#include "User.h"

enum DataType	//	Not used anymore
{
	MESSAGE,
	PROFILE,

	NUM_DATATYPE
};

class DataToSend
{
public:
	//DataType type;	Not used anymore
	//int userId = 0;

	DataToSend();

	string userName;
	Color userColor;
	Color userMessageColor;
	string message;
};

