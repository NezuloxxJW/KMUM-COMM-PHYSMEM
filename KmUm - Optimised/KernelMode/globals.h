#pragma once
#include "driver.h"

int oPid; // overlay/usermode process ID
uintptr_t oAddr; // overlay/usermode base struct address

int gPid; // game process ID