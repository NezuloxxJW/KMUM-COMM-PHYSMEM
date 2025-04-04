#include "driver.h"

struct commAddr {
    uintptr_t pFct; // int
    uintptr_t pPid; // int
    uintptr_t pAddr; // uintptr_t
    uintptr_t pSize; // size_t
    uintptr_t pValue; // ULONGLONG
    uintptr_t pKM; // response
    uintptr_t pEnd; // bool
};

struct respAddr {
    uintptr_t pBuff; // PVOID
    uintptr_t pStatus; // int
    uintptr_t pEnd; // bool
};

commAddr cAddr;
respAddr rAddr;


struct response {
    PVOID buffer;
    int status;
    bool end;
};

struct communication
{
    int fct; // 10 = Read process memory, 20 = Write process memory
    int pid;
    uintptr_t addr;
    SIZE_T size;
    ULONGLONG value;
    response KMresponse;
    bool end;
};

communication comm;