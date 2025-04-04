#include "driver.h"
#include "communication.hpp"

// redirect request
NTSTATUS main() {
    bool UMalive = true;

    while (UMalive) {
        usermode::readRequest();
        switch (comm.fct){
        case 10: game::read(); break;
        case 20: game::write(); break;
        case 420: usermode::exit(); UMalive = false; break;
        }

        delay(10);
    }

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject); UNREFERENCED_PARAMETER(RegistryPath);

    if (!NT_SUCCESS(usermode::InitializeCommunication())) { return STATUS_ABANDONED; }

    return main();
}