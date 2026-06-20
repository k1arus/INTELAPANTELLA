#include "SerialBridge.h"

#include <windows.h>
#include <string>

static HANDLE serialHandle = INVALID_HANDLE_VALUE;
static std::string receiveBuffer;

bool openSerial()
{
    serialHandle = CreateFile(
        "\\\\.\\COM3",
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    if (serialHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DCB dcbSerialParams = {};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(
            serialHandle,
            &dcbSerialParams))
    {
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(
            serialHandle,
            &dcbSerialParams))
    {
        CloseHandle(serialHandle);
        serialHandle = INVALID_HANDLE_VALUE;
        return false;
    }

    COMMTIMEOUTS timeouts = {};

    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;

    SetCommTimeouts(
        serialHandle,
        &timeouts);

    return true;
}

bool receivedSuccess()
{
    if (serialHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD bytesRead = 0;
    char ch;

    if (!ReadFile(
            serialHandle,
            &ch,
            1,
            &bytesRead,
            nullptr))
    {
        return false;
    }

    if (bytesRead == 0)
    {
        return false;
    }

    if (ch == '\r')
    {
        return false;
    }

    if (ch == '\n')
    {
        bool success =
            (receiveBuffer == "Success");

        receiveBuffer.clear();

        return success;
    }

    receiveBuffer += ch;

    return false;
}
