# KMUM-COMM-PHYSMEM
**Kernel mode communication with usermode using contiguous memory**  
*This works for signed, test-signed and manual mapped kernel drivers*

---

## 📅 Release Date  
2025-04-04 | Proof-of-Concept v1.0

---

## 🔧 Technical Principle  
Utilizes contiguous memory structures in C++ where sequential variables maintain predictable offsets.  

**Workflow**:  
1. User-mode sends virtual base address of structure via Registry
2. Kernel get and translates virtual → physical address  
3. `MmCopyMemory` with `PHYSICAL` flag safely accesses physical memory
4. Execute request

---

## ⚠️ Precautions

### 1. **Usermode and Kernelmode Overlap**:  
   Usermode and kernelmode must NOT override themselves. Otherwise, you will lose your communication and need to reinitialize it.

### 2. Performance Considerations  
- **Recommended**: Dedicated comm thread (kernel-side)  
- **Manual Mapping**: Be aware of potential flag conflicts
- **Speed** : Removing sleeps will make the comm faster but use more ressources

---

## 🏆 Why use this?

- No need for IOCTL
- No need for syscall hook
- No need to create section

---

-- Nezuloxx 
