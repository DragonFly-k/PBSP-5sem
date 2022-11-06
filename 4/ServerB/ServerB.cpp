#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <time.h>
#include <tchar.h>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "ErrorMsgText.h"

SOCKET sS;
SOCKET cC;
static int count = 0;

bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen)
{
	char nameServer[50];
	memset(from, 0, sizeof(flen));
	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;
	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		throw  SetErrorMsgText("bind:", WSAGetLastError());
	if ((recvfrom(sS, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT) return false;
		else throw  SetErrorMsgText("recv:", WSAGetLastError());
	}
	if (strcmp(nameServer, name) == 0)
	{
		SOCKADDR_IN* addr = (SOCKADDR_IN*)&from;
		std::cout << std::endl << "Порт клиента: " << addr->sin_port;
		std::cout << std::endl << "IP-адрес клиента: " << inet_ntoa(addr->sin_addr);
		std::cout << std::endl << "Позывной сервера совпадает" << std::endl;
		return true;
	}
	else return false;
}

bool PutAnswerToClient(const char* name, sockaddr* to, int* lto)
{
	if ((sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
		throw  SetErrorMsgText("send:", WSAGetLastError());
	return true;
}

int main()
{
	setlocale(LC_CTYPE, "rus");
	WSADATA wsaData;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		char name[] = "Katy";
		for (;;)
		{
			SOCKADDR_IN clnt;
			int lc = sizeof(clnt);
			if (GetRequestFromClient(name, 3000, (sockaddr*)&clnt, &lc))
				PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
			if (closesocket(sS) == SOCKET_ERROR)
				throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		}
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
	}
	catch (std::string errorMsgText)
	{
		std::cout << std::endl << errorMsgText;
	}

	std::cout << std::endl;
	return 0;
}