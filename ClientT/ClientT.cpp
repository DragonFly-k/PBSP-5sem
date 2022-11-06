#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Ws2tcpip.h"
#include "ErrorMsgText.h"
using namespace std;

// -- установить соединение с сокетом
//int connect(
//    SOCKET s,              // [in] дескриптор связанного сокета
//    struct sockaddr_in* a, // [in] указатель на SOCKADDR_IN 
//    int  la                // [in] длина SOCKADDR_IN в байтах                
//);

int main()
{
    setlocale(LC_ALL, "Russian");
    WSADATA wsaData;
    SOCKET  cC;
    SOCKADDR_IN serv;                    
    serv.sin_family = AF_INET;            
    serv.sin_port = htons(2000);                   
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");   
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());
        if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());
        if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
            throw  SetErrorMsgText("connect:", WSAGetLastError());
        char message[50] = "Hello from Client ";
        int countMessage = 0;
        std::cout << std::endl << "кол-во сообщений: ";
        std::cin >> countMessage;
        for (int i = 0; i < countMessage; i++)
        {
            char newMessage[50] = "";
            char number[5] = "";
            _itoa(i, number, 10);
            strcat(newMessage, message);
            strcat(newMessage, number);
            if ((send(cC, newMessage, strlen(newMessage) + 1, NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("send:", WSAGetLastError());
            if ((recv(cC, newMessage, sizeof(newMessage), NULL)) == SOCKET_ERROR)
                throw  SetErrorMsgText("recv:", WSAGetLastError());
            std::cout << std::endl << "от сервера: " << newMessage;
        }
        if ((send(cC, "end", strlen("end"), NULL)) == SOCKET_ERROR)
            throw  SetErrorMsgText("send:", WSAGetLastError());
        if (closesocket(cC) == SOCKET_ERROR)
            throw  SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }
    cout << endl << endl;
    system("pause");
}