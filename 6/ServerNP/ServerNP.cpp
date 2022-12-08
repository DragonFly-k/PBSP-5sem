#include <iostream>
#include <clocale>
#include <ctime>
#include "ErrorMsgText.h"
#include "Windows.h"

#define NAME L"\\\\.\\pipe\\Tube"

// --создать именованный канал
//HANDLE CreateNamedPipe
//(
//    LPCTSTR pname, // [in] символическое имя канала 
//    DWORD omode, // [in] атрибуты канала
//    DWORD pmode, // [in] режимы передачи данных
//    DWORD pimax, // [in] макс. к-во экземпляров канала 
//    DWORD timeo, // [in] время ожидания связи с клиентом
//);
// Код возврата: в случае успешного завершения функция 
// возвращает дескриптор именованного канала, иначе
// pmode – задает флаги способов передачи данных,
// например, флаг PIPE_TYPE_MESSAGE|PIPE_WAIT разрешает 
// запись данных сообщениями в синхронном режиме,
//BOOL ReadFile
//(
//    HANDLE hP, // [in] дескриптор канала
//    LPVOID pb, // [out] указатель на буфер ввода
//    DWORD sb, // [in] количество читаемых байт
//    LPDWORD ps, // [out] количество прочитанных байт
//); 
//BOOL WriteFile
//(
//    HANDLE hP, // [in] дескриптор канала
//    LPVOID pb, // [in] указатель на буфер вывода
//    DWORD sb, // [in] количество записываемых байт
//    LPDWORD ps, // [out] количество записанных байт
//);

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    HANDLE sH;
    DWORD lp;
    char buf[50];
    try {
        cout << "ServerNP\n\n";
        if ((sH = CreateNamedPipe(NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, NULL, NULL, INFINITE, NULL)) == INVALID_HANDLE_VALUE)
            throw SetPipeError("CreateNamedPipe: ", GetLastError());
        while (true) {
            cout << "Awaiting connection...\n\n";
            if (!ConnectNamedPipe(sH, NULL))
                throw SetPipeError("ConnectNamedPipe: ", GetLastError());
            while (true) {
                if (ReadFile(sH, buf, sizeof(buf), &lp, NULL)) {
                    if (strcmp(buf, "STOP") == 0) {
                        cout << endl;
                        break;
                    }
                    cout << buf << endl;
                    if (WriteFile(sH, buf, sizeof(buf), &lp, NULL)) {
                        if (strstr(buf, "ClientNPct")) break;
                    }
                    else throw SetPipeError("WriteFile: ", GetLastError());
                }
                else throw SetPipeError("ReadFile: ", GetLastError());
            }
            if (!DisconnectNamedPipe(sH))
                throw SetPipeError("DisconnectNamedPipe: ", GetLastError());
        }
        if (!CloseHandle(sH))
            throw SetPipeError("CloseHandle: ", GetLastError());
        system("pause");
    }
    catch (string ErrorPipeText) {
        cout << endl << ErrorPipeText;
    }
}