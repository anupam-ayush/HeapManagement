# Custom Heap Memory Manager in C

A lightweight, bare-metal implementation of dynamic memory allocation (`malloc` and `free` equivalents) in C. Designed without relying on standard library memory calls (`sbrk`, `brk`, or system `malloc`), this allocator operates over a pre-allocated static 1024-byte memory buffer.

---

## 📌 Features

- **Implicit Free List:** Traverses contiguous header-payload partitions without separate linked list nodes.
- **First-Fit Search Strategy:** Fast block discovery for allocation requests.
- **Dynamic Block Splitting:** Avoids internal fragmentation by carving exact payload sizes and converting surpluses into new free blocks.
- **4-Byte Boundary Alignment:** Automatic upward rounding of payload requests using bitwise masking (`(size + 3) & ~3`).
- **Immediate Adjacent Coalescing:** Eliminates external fragmentation upon deallocation by merging neighboring free memory segments.
---
