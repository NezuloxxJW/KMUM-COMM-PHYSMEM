#include "structs.h"
#include "registry.h"

int WkernelResponse();
bool initCommunication();

// read template
template <typename T>
T read(uintptr_t addr) {
    comm.fct = 10;
    comm.addr = addr;
    comm.size = sizeof(T);

    if (WkernelResponse() != 0) { return -1; }

    T rtn = *(T*)&comm.KMresponse.buffer;
    return rtn;
}

// write template
template <typename T>
void write(uintptr_t addr, T value) {
    comm.fct = 20;
    comm.addr = addr;
    comm.value = value;
    comm.size = sizeof(T);

    WkernelResponse();
}

void exit();