#include "Winsock2.h"               
#pragma comment(lib, "WS2_32.lib") 
#pragma warning(disable : 4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <clocale>
#include <ctime>
#include "ErrorMsgText.h"

  
// -- создать сокет 
//SOCKET socket(
//    int    af,   //[in]  формат адреса  
//    int    type, //[in]  тип сокета   
//    int    prot  //[in]  протокол
//);      
//   
// -- закрыть существующий  сокет 
//SOCKET closesocket(
//    SOCKET s,    //[in] дескриптор сокета   
//    );
// 
// -- связать сокет с параметрами 
//int bind(
//    SOCKET s,                    //[in] сокет
//    cost struct sockaddr_in* a, //[in] указатель на SOCKADDR_IN
//    int    la                    //[in] длина SOCKADDR_IN в байтах
//)
//     
// -- переключить сокет в режим прослушивания
//int listen(
//    SOCKET s,    //[in] дескриптор связанного сокета
//    int    mcq,  //[in] максимальная длина очереди                 
//    ); 
// 
// -- разрешить подключение к сокету  
//SOKET accept(
//    SOCKET s,             // [in]  дескриптор связанного сокета
//    struct sockaddr_in* a,//[out] указатель на SOCKADDR_IN 
//    int* la               //[out] указатель на длину SOCKADDR_IN                 
//);
//        
// -- отправить данные по установленному каналу 
//int send(
//    SOCKET s,        // [in] дескриптор сокета (канал для передачи) 
//    const char* buf, // [in] указатель буфер данных
//    int  lbuf,       // [in] количество байт данных в буфере
//    int  flags       // [in] индикатор особого режима маршрутизации      
//);
//  
// -- принять данные по установленному каналу
//int recv(
//    SOCKET s,        // [in] дескриптор сокета (канал для приема) 
//    const char* buf, // [in] указатель буфер данных
//    int  lbuf,       // [in] количество байт данных в буфере
//    int  flags       // [in] индикатор  
//);

int main()
{
    setlocale(LC_ALL, "rus");
    WSADATA wsaData;
    SOCKET  sS;   
    SOCKADDR_IN serv;               
    serv.sin_family = AF_INET;      
    serv.sin_port = htons(2000);   
    serv.sin_addr.s_addr = INADDR_ANY;   
    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
            throw  SetErrorMsgText("Startup: ", WSAGetLastError());
        }
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) {
            throw  SetErrorMsgText("socket: ", WSAGetLastError());
        }
        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) {
            throw  SetErrorMsgText("bind: ", WSAGetLastError());
        }
        if (listen(sS, SOMAXCONN) == SOCKET_ERROR) {
            throw SetErrorMsgText("listen: ", WSAGetLastError());
        }
		std::cout << "Сервер запущен. Ожидание подключения клиента..." << std::endl;
        SOCKET cS;      
        SOCKADDR_IN clnt;      
        memset(&clnt, 0, sizeof(clnt));    
        int lclnt = sizeof(clnt); 
        while (true) {
            if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET) 
                throw SetErrorMsgText("accept: ", WSAGetLastError());
            std::cout << std::endl << "IP-client: " << inet_ntoa(clnt.sin_addr);
            std::cout << std::endl << "Port-client: " << clnt.sin_port << std::endl;
            clock_t start = clock();
            while (true) {
                char message[50] = "";
                if ((recv(cS, message, sizeof(message), NULL)) == SOCKET_ERROR)
                    throw  SetErrorMsgText("recv:", WSAGetLastError());
                std::cout << std::endl << "от клиента: " << message;
                if (strcmp(message, "end") == 0)
                    break;
                if ((send(cS, message, strlen(message) + 1, NULL)) == SOCKET_ERROR)
                    throw  SetErrorMsgText("send:", WSAGetLastError());
            }
            clock_t finish = clock();
            std::cout << std::endl << "Затраченное время: " << finish - start << std::endl << std::endl;
        }
        if (closesocket(sS) == SOCKET_ERROR) {
            throw  SetErrorMsgText("closesocket: ", WSAGetLastError());
        }
        if (WSACleanup() == SOCKET_ERROR) {
            throw  SetErrorMsgText("Cleanup: ", WSAGetLastError());
        }
    }
    catch (std::string errorMsgText) {
        WSACleanup();
        std::cout << '\n' << errorMsgText;
    }
    system("pause");
    return 0;
}