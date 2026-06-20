#include "SerialBridge.h"
#include <windows.h>

static HANDLE serialHandle = INVALID_HANDLE_VALUE;

bool openSerial() {
    serialHandle = CreateFile("\\\\.\\COM3", GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);

    return serialHandle != INVALID_HANDLE_VALUE;
}

bool receivedSuccess() {
    return false;
}
