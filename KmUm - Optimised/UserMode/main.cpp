#include "misc.h"
#include "kernel.h"

communication comm;

int main()
{
    // initialise communication with kernel mode
    if (!initCommunication()) { return -1; }

    if (!cleanRegistry()) { std::cout << "[!] Failed to clean, skip" << std::endl; }
    else { std::cout << "[+] Registry has been cleaned" << std::endl; }

    comm.pid = getProcessid(L"UserMode.exe");
    Sleep(1000);

    // loop values
    int idx = 0;
    int err = 0;

    // test
    float testNumber = 13.99027;

    srand(time(0));
    for (idx; idx < 100; idx++) {
        auto start = std::chrono::high_resolution_clock::now();

        float testValue = read<float>(reinterpret_cast<uintptr_t>(&testNumber));

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

        std::cout << "Operation took : " << duration << " ns" << std::endl;

        if (testNumber != testValue) {
            err++;
            std::cout << "[-] Error " << err << " found at index : " << idx << std::endl;
        }
        testNumber = randomFloat();

        Sleep(10);
    }

    exit();
    Sleep(10000);
}