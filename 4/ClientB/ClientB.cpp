#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <tchar.h>
#include "Winsock2.h"
#include "ErrorMsgText.h"
#pragma comment(lib, "WS2_32.lib")

SOCKET cC;

bool GetServer(char* call, short port, sockaddr* from, int* flen)
{
	memset(from, 0, sizeof(flen));
	if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	int optval = 1;
	if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		throw  SetErrorMsgText("opt:", WSAGetLastError());
	SOCKADDR_IN all;
	all.sin_family = AF_INET;
	all.sin_port = htons(port);
	all.sin_addr.s_addr = INADDR_BROADCAST;
	if ((sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
		throw  SetErrorMsgText("sendto:", WSAGetLastError());
	char nameServer[50];
	if ((recvfrom(cC, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT) return false;
		else throw  SetErrorMsgText("recv:", WSAGetLastError());
	}
	if (strcmp(nameServer, call) == 0)
	{
		SOCKADDR_IN* addr = (SOCKADDR_IN*)&from;
		std::cout << std::endl << "Порт сервера: " << addr->sin_port;
		std::cout << std::endl << "IP-адрес сервера: " << inet_ntoa(addr->sin_addr);
		std::cout << std::endl << "Сервер с таким именем найден";
		return true;
	}
	else
	{
		std::cout << std::endl << "Сервер с таким именем не найден";
		return false;
	}
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
		char call[] = "Katy";
		SOCKADDR_IN clnt;
		int lc = sizeof(clnt);
		GetServer(call, 3000, (sockaddr*)&clnt, &lc);
		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (std::string errorMsgText)
	{
		std::cout << std::endl << errorMsgText;
	}
	std::cout << std::endl;
	return 0;
}