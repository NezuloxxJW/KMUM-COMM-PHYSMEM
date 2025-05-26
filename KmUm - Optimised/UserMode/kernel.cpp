#include "kernel.h"

// Waiting for the kernel mode response
int waitKernelResponse() {
    while (!comm.kmResponse.end == true) { Sleep(10); }
    comm.kmResponse.end = false;

    return comm.kmResponse.status;
}

// initialisation for the communication with kernel mode
bool initCommunication() {
    // Way to send data to kernel mode
    if (!writeRegistryDword(HKEY_LOCAL_MACHINE, L"", L"oPid", GetCurrentProcessId())) { return false; }
    if (!writeRegistryQword(HKEY_LOCAL_MACHINE, L"", L"oAddr", reinterpret_cast<DWORD_PTR>(&comm))) { return false; }

    // Waiting for the kernel mode response
    if (waitKernelResponse() != 0) { return false; }

    return true;
}

void exit() {
    comm.fct = 420;

    waitKernelResponse();
    std::cout << "[+] Goodbye kernel !" << std::endl;
}