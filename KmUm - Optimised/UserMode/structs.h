#include <Windows.h>

struct response {
    PVOID buffer;
    int status;
    bool end;
};

struct communication
{
    int fct;
    int pid;
    uintptr_t addr;
    SIZE_T size;
    ULONGLONG value;
    response KMresponse;
    bool end;
};



extern struct communication comm;