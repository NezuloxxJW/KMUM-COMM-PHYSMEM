#include "driver.h"
#include "communication.hpp"

// redirect request
NTSTATUS main() {
    bool isRunning = true;

    while (isRunning) {
        usermode::readRequest();
        switch (comm.fct){
        case 10: game::read(); break;
        case 20: game::write(); break;
        case 420: usermode::exit(); isRunning = false; break;
        }

        delay(10);
    }

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath)
{
    UNREFERENCED_PARAMETER(driverObject); UNREFERENCED_PARAMETER(registryPath);

    if (!NT_SUCCESS(usermode::initializeCommunication())) { return STATUS_ABANDONED; }

    return main();
}