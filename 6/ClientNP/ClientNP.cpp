#include <iostream>
#include <clocale>
#include <ctime>

#include "ErrorMsgText.h"
#include "Windows.h"

//#define NAME L"\\\\.\\pipe\\Tube"
#define NAME L"\\\\DESKTOP-M01CN9D\\pipe\\Tube"

#define STOP "STOP"

// -- открыть канал
//HANDLE CreateFile
//(
//    LPCTSTR pname, // [in] символическое имя канала 
//    DWORD accss, // [in] чтение или запись в канал
//    DWORD share, // [in] режим совместного использования
//    DWORD oflag, // [in] флаг открытия канала
//    );
// Код возврата: в случае успешного завершения функция 
// возвращает дескриптор именованного канала,
// GENERIC_READ| GENERIC_WRITE (запись,чтение) 
// FILE_SHARE_READ| FILE_SHARE_WRITE (совместного чтение и запись);
// OPEN_EXISTING(открытие существующего канала);

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    HANDLE cH;
    DWORD lp;
    char buf[50];
    try {
        cout << "ClientNP\n\n";
        if ((cH = CreateFile(NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
            throw  SetPipeError("CreateFile: ", GetLastError());
        int countMessage;
        cout << "Number of messages: ";
        cin >> countMessage;
        for (int i = 1; i <= countMessage; i++) {
            string obuf = "Hello from ClientNP " + to_string(i);
            if (!WriteFile(cH, obuf.c_str(), sizeof(obuf), &lp, NULL))
                throw SetPipeError("WriteFile: ", GetLastError());
            if (!ReadFile(cH, buf, sizeof(buf), &lp, NULL))
                throw SetPipeError("ReadFile: ", GetLastError());
            cout << buf << endl;
        }
        if (!WriteFile(cH, STOP, sizeof(STOP), &lp, NULL))
            throw SetPipeError("WriteFile: ", GetLastError());
        if (!CloseHandle(cH))
            throw SetPipeError("CloseHandle: ", GetLastError());
        system("pause");
    }
    catch (string ErrorPipeText) {
        cout << endl << ErrorPipeText;
    }
}