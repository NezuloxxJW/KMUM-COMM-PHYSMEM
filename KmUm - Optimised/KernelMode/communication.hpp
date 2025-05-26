#pragma once
#include "driver.h"
#include "Ustruct.h"
#include "registry.hpp"
#include "memory.hpp"

namespace usermode {
    void updateAddr() {
        //cAddr -> usermode communication struct base address
        cAddr.pFct = oAddr;
        cAddr.pPid = cAddr.pFct + sizeof(int);
        cAddr.pAddr = cAddr.pPid + sizeof(int);
        cAddr.pSize = cAddr.pAddr + sizeof(uintptr_t);
        cAddr.pValue = cAddr.pSize + sizeof(SIZE_T);
        cAddr.pKM = cAddr.pValue + sizeof(ULONGLONG);
        cAddr.pEnd = cAddr.pKM + sizeof(response);
        //rAddr -> usermode response struct base address
        rAddr.pBuff = cAddr.pKM;
        rAddr.pStatus = rAddr.pBuff + sizeof(ULONGLONG);
        rAddr.pEnd = rAddr.pStatus + sizeof(int);
    }

    // read whole UM structure
    void readRequest() {
        SIZE_T wByte;

        NTSTATUS status = ReadProcessMemory(
            (HANDLE)oPid,
            reinterpret_cast<PVOID>(cAddr.pFct),
            &comm,
            sizeof(comm),
            &wByte
        );

        return;
    }

    template <typename T>
    T read(uintptr_t address) {
        SIZE_T wByte;
        T value;

        NTSTATUS status = ReadProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(address),
            &value,
            sizeof(T),
            &wByte
        );
        if (!NT_SUCCESS(status)) { return NULL; }

        return value;
    }

    void readGpid() {
        SIZE_T wByte;
        int gamePid = 0;

        while (gamePid == 0) {
            NTSTATUS status = ReadProcessMemory(
                reinterpret_cast<HANDLE>(oPid),
                reinterpret_cast<PVOID>(cAddr.pPid),
                &gamePid,
                sizeof(int),
                &wByte
            );

            delay(10);
        }
        gPid = gamePid;
    }

    // send buffer to UM
    NTSTATUS sendBuffer(PVOID value) {
        SIZE_T wByte;
        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(rAddr.pBuff),
            &value,
            sizeof(read<size_t>(cAddr.pSize)),
            &wByte
        );

        return status;
    };

    // send end to UM
    NTSTATUS end(bool end = true) {
        SIZE_T wByte;
        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(rAddr.pEnd),
            &end,
            sizeof(bool),
            &wByte
        );

        return status;
    }

    // Connection initlisation with usermode
    NTSTATUS initializeCommunication() {
        NTSTATUS status = readAddressFromRegistry();
        if (!NT_SUCCESS(status)) {
            DbgPrint("Failed to read Addr from registry\n");
            return status;
        }
        status = readPidFromRegistry();
        if (!NT_SUCCESS(status)) {
            DbgPrint("Failed to read PID from registry\n");
            return status;
        }

        usermode::updateAddr();
        status = usermode::end();
        usermode::readGpid();
        return status;
    }

    void exit() {
        DbgPrint("Goodbye ! \n");
        end();
    }
}

namespace game {
    NTSTATUS read() {
        SIZE_T wByte;
        PVOID value = 0;
        
        NTSTATUS status = ReadProcessMemory(
            reinterpret_cast<HANDLE>(gPid),
            reinterpret_cast<PVOID>(comm.addr),
            &value,
            comm.size,
            &wByte
        );
        if (!NT_SUCCESS(status)) { return status; }

	status = usermode::sendBuffer(value);
        if (!NT_SUCCESS(status)) { return status; }

        return usermode::end();
    };

    NTSTATUS write() {
        SIZE_T wByte;
        PVOID value = reinterpret_cast<PVOID>(comm.value);

        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(gPid),
            reinterpret_cast<PVOID>(comm.addr),
            &value,
            comm.size,
            &wByte
        );
        if (!NT_SUCCESS(status)) { return status; }

        return usermode::end();
    };
}

