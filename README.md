# KMUM-COMM-RPM
Kernel mode communication from usermode using physical memory.
This works for signed, test-signed and manual mapped kernel drivers.

POC : released soon

Managed structure in c++ will always link to next variable.

From that we can send struct base address (virtual, since usermode) to the kernelmode.

Kernelmode will translate virtual address, since we don't want any hooked function from Anti-Cheat, to physical.

And then use MmCopyMemory with physical flag to read the command asked by the usermode.


Usermode and kernelmode will have to NOT override them self. Otherwise you can say goodbye to your communication and need to reinitialise it.

For that we'll use a security system.


For best performances, I do recommend to have a thread running but it'll had flags if you're manually mapped.

But it'll work pretty good in a loop too.


Why using this ?
You can't use IOCTL with a manual mapped driver.
Better than hooking a syscall.
You don't need shared memory.
