#include <iostream>
#include <clocale>
#include <ctime>
#include "ErrorMsgText.h"
#include "Winsock2.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "WS2_32.lib")  

//int recvfrom(
//    SOCKET s,              // [in] дескриптор сокета
//    char* buf,             // [out] буфер для получаемых данных  
//    int          len,      // [in] размер буфера buf
//    int          flags,    // [in] индикатор режима маршрутизации  
//    struct sockaddr* to,   // [out] указатель на SOCKADDR_IN  
//    int* tolen             // [out] указатель на размер to 
//);
//int sendto(
//    SOCKET s,              // [in] дескриптор сокета
//    const char* buf,       // [in] буфер для пересылаемых данных
//    int len,               // [in] размер буфера buf
//    int flags,             // [in] индикатор режима маршрутизации 
//    const struct sockaddr* to, // [in] указатель на SOCKADDR_IN 
//    int tolen              // [in] длина структуры to 
//);

int main()
{
    setlocale(LC_ALL, "Russian");
    SOCKET  sS;
    WSADATA wsaData; 
    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(2000);
    serv.sin_addr.s_addr = INADDR_ANY;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw  SetErrorMsgText("Startup:", WSAGetLastError());
        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw  SetErrorMsgText("socket:", WSAGetLastError());
        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw  SetErrorMsgText("bind:", WSAGetLastError());
        		
        SOCKADDR_IN clnt;
        memset(&clnt, 0, sizeof(clnt));
        int lc = sizeof(clnt);
        while (1)
        {
            clock_t  start = clock();
            while (1)
            {
                char ibuf[50] = "";
                int  lb = 0;
                if (lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc) == SOCKET_ERROR)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());
                cout << "получено от клиента: " << ibuf << endl;
                if (strcmp(ibuf, "end") == 0)  break;
                //Sleep(1);
                int  lobuf = 0;
                if ((lobuf = sendto(sS, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());
            }
            clock_t finish = clock();
            cout << endl << "Затраченное время: " << finish - start << endl << endl;
        }
        if (closesocket(sS) == SOCKET_ERROR)
            throw  SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw  SetErrorMsgText("cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }
    cout << endl << endl;
    system("pause");
}