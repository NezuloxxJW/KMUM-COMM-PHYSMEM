# KMUM-COMM-RPM
Kernel mode communication from usermode using physical memory.  
This works for signed, test-signed and manual mapped kernel drivers.

---

## POC  
Released soon.

---

## Principle

Managed structure in C++ will always link to the next variable.  
From that, we can send the struct base address (virtual, since usermode) to the kernelmode.  
Kernelmode will translate the virtual address (to avoid any hooked function from Anti-Cheat) into physical.  
Then it will use `MmCopyMemory` with the physical flag to read the command requested by usermode.

---

## Precautions

1. **Usermode and Kernelmode Overlap**:  
   Usermode and kernelmode must NOT override themselves. Otherwise, you will lose your communication and need to reinitialize it. To prevent this, a security system will be implemented.

2. **Performance Optimization**:  
   For best performance, it is recommended to have a dedicated thread running (though this may add flags if you are manually mapped).  
   However, it will also work effectively in a loop.

---

## Why use this?

- You can't use IOCTL with a manually mapped driver.  
- It's better than hooking a syscall.  
- You don't need shared memory.

---
