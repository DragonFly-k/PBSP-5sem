#include <iostream>
#include <clocale>
#include <ctime>

#include "ErrorMsgText.h"
#include "Windows.h"

#define STOP "STOP"
#define NAME L"\\\\.\\pipe\\Tube"

// -- изменить характеристики канала
// Назначение: функция предназначена для изменения 
// динамических характеристик именованного канала 
//BOOL SetNamedPipeHandleState
//(
//    HANDLE hP, // [in] дескриптор именованного канала
//    LPDWORD pst, // [in] указатель на новое состояние канала
//);
// -- писать и читать данные канала
//BOOL TransactNamedPipe
//(
//    HANDLE hP, // [in] дескриптор канала
//    LPVOID pw, // [in] указатель на буфер для записи
//    DWORD sw, // [in] размер буфера для записи
//    LPVOID pr, // [out] указатель на буфер для чтения 
//    DWORD sr, // [in] размер буфера для чтения
//    LPDWORD pr, // [out] количество прочитанных байт
//);

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    HANDLE cH;
    DWORD mode = PIPE_READMODE_MESSAGE;
    DWORD lp;
    char ibuf[50] = "Hello from Client",
        obuf[50];

    try {
        cout << "ClientNPt\n\n";
        if ((cH = CreateFile(NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE)
            throw SetPipeError("CreateFile: ", GetLastError());
        if (!SetNamedPipeHandleState(cH, &mode, NULL, NULL)) {
            cout << GetLastError();
            throw SetPipeError("SetNamedPipeHandleState: ", GetLastError());
        }
        int countMessage;
        cout << "Number of messages: ";
        cin >> countMessage;
        for (int i = 1; i <= countMessage; i++) {
            string obufstr = "Hello from ClientNPt " + to_string(i);
            strcpy_s(obuf, obufstr.c_str());
            if (!TransactNamedPipe(cH, obuf, sizeof(obuf), ibuf, sizeof(ibuf), &lp, NULL))
                throw SetPipeError("TransactNamedPipe: ", GetLastError());
            cout << ibuf << endl;
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