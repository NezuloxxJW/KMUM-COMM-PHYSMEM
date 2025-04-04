#include "misc.h"
#include "kernel.h"

communication comm;

int main()
{
    // initialise communication with kernel mode
    std::cout << "[~] Waiting for kernel" << std::endl;
    if (!initCommunication()) { return -1; }
    std::cout << "[+] Communication with kernel has been established" << std::endl;

    std::cout << "[~] Cleaning registry" << std::endl;
    if (!CleanRegistry()) { std::cout << "[-] Failed to clean, skip" << std::endl; }
    else { std::cout << "[+] Registry has been cleaned" << std::endl; }

    std::cout << "[~] Get proc id" << std::endl;
    comm.pid = getprocessid(L"UserMode.exe");

    Sleep(1000);

    // test
    float TestNumber = 13.99027;
    int idx = 0;
    int err = 0;

    srand(time(0));

    for (idx; idx < 100; idx++) {
        std::cout << "[~] For IDX : " << idx << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        float TestValue = read<float>(reinterpret_cast<uintptr_t>(&TestNumber));

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

        std::cout << "Operation took : " << duration << " ns" << std::endl;

        if (TestNumber != TestValue) {
            err++;
            std::cout << "[-] Error " << err << " found at index : " << idx << std::endl;
        }
        TestNumber = randomFloat();

        Sleep(10);
    }

    exit();
    Sleep(10000);
}