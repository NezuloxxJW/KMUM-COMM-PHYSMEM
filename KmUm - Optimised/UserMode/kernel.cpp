#include "kernel.h"

// Waiting for the kernel mode response
int WkernelResponse() {
    while (!comm.KMresponse.end == true) { Sleep(10); }
    comm.KMresponse.end = false;

    return comm.KMresponse.status;
}

// initialisation for the communication with kernel mode
bool initCommunication() {
    // Way to send data to kernel mode
    if (!WriteRegistryDword(HKEY_LOCAL_MACHINE, L"", L"oPid", GetCurrentProcessId())) { return false; }
    if (!WriteRegistryQword(HKEY_LOCAL_MACHINE, L"", L"oAddr", reinterpret_cast<DWORD_PTR>(&comm))) { return false; }

    // Waiting for the kernel mode response
    if (WkernelResponse() != 0) { return false; }

    return true;
}

void exit() {
    comm.fct = 420;

    WkernelResponse();
    std::cout << "[+] Goodbye kernel !" << std::endl;
}