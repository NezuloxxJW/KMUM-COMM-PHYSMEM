#pragma once
#include "driver.h"
#include "Ustruct.h"
#include "registry.hpp"
#include "memory.hpp"

namespace usermode {
    void updateAddr() {
        //cAddr
        cAddr.pFct = oAddr;
        cAddr.pPid = cAddr.pFct + sizeof(int);
        cAddr.pAddr = cAddr.pPid + sizeof(int);
        cAddr.pSize = cAddr.pAddr + sizeof(uintptr_t);
        cAddr.pValue = cAddr.pSize + sizeof(SIZE_T);
        cAddr.pKM = cAddr.pValue + sizeof(ULONGLONG);
        cAddr.pEnd = cAddr.pKM + sizeof(response);
        //rAddr
        rAddr.pBuff = cAddr.pKM;
        rAddr.pStatus = rAddr.pBuff + sizeof(ULONGLONG);
        rAddr.pEnd = rAddr.pStatus + sizeof(int);
    }

    // read whole UM structure (otherwise you'll have 2/3 read usermode before executing the one for the game)
    void readRequest() {
        SIZE_T WByte;
        int Request = 0;

        NTSTATUS status = ReadProcessMemory(
            (HANDLE)oPid,
            reinterpret_cast<PVOID>(cAddr.pFct),
            &comm,
            sizeof(comm),
            &WByte
        );
        if (!NT_SUCCESS(status)) { return; }

        return;
    }

    template <typename T>
    T read(uintptr_t address) {
        SIZE_T WByte;
        T value;

        NTSTATUS status = ReadProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(address),
            &value,
            sizeof(T),
            &WByte
        );
        if (!NT_SUCCESS(status)) { return NULL; }

        return value;
    }

    void ReadGPid() {
        SIZE_T WByte;
        int GamePid = 0;

        while (GamePid == 0) {
            NTSTATUS status = ReadProcessMemory(
                reinterpret_cast<HANDLE>(oPid),
                reinterpret_cast<PVOID>(cAddr.pPid),
                &GamePid,
                sizeof(int),
                &WByte
            );

            delay(10);
        }
        gPid = GamePid;
    }

    // send buffer to UM
    NTSTATUS sendBuffer(PVOID value) {
        SIZE_T WByte;
        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(rAddr.pBuff),
            &value,
            sizeof(read<size_t>(cAddr.pSize)),
            &WByte
        );

        return status;
    };

    // end KMresponse for UM
    NTSTATUS end(bool end = true) {
        SIZE_T WByte;
        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(oPid),
            reinterpret_cast<PVOID>(rAddr.pEnd),
            &end,
            sizeof(bool),
            &WByte
        );

        return status;
    }

    // Connection initlisation with usermode
    NTSTATUS InitializeCommunication() {
        NTSTATUS status = ReadRegistryADDR();
        if (!NT_SUCCESS(status)) {
            DbgPrint("Failed to read Addr from registry\n");
            return status;
        }
        status = ReadRegistryPID();
        if (!NT_SUCCESS(status)) {
            DbgPrint("Failed to read PID from registry\n");
            return status;
        }

        usermode::updateAddr();
        status = usermode::end();
        usermode::ReadGPid();
        return status;
    }

    void exit() {
        DbgPrint("Goodbye ! \n");
        end();
    }
}

namespace game {
    NTSTATUS read() {
        SIZE_T WByte;
        PVOID value = 0;
        
        NTSTATUS status = ReadProcessMemory(
            reinterpret_cast<HANDLE>(gPid),
            reinterpret_cast<PVOID>(comm.addr),
            &value,
            comm.size,
            &WByte
        );
        if (!NT_SUCCESS(status)) { return status; }

        status = usermode::sendBuffer(value);
        if (!NT_SUCCESS(status)) { return status; }

        status = usermode::end();

        return status;
    };

    NTSTATUS write() {
        SIZE_T WByte;
        PVOID value = reinterpret_cast<PVOID>(comm.value);

        NTSTATUS status = WriteProcessMemory(
            reinterpret_cast<HANDLE>(gPid),
            reinterpret_cast<PVOID>(comm.addr),
            &value,
            comm.size,
            &WByte
        );
        if (!NT_SUCCESS(status)) { return status; }

        status = usermode::end();

        return status;
    };
}

