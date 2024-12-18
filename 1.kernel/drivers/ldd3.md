# Linux Driver Development E3
http://lwn.net/Kernel/LDD3/
http://lwn.net/images/pdf/LDD3/ldd3_pdf.tar.bz2

+ A device driver is providing mechanism, not policy.
+ Most programming problems can be splited into two parts:
    1. what capabilities are to be provided (mechanism)
    2. how capabilities can be used (policy)

+ Linux driver developers should pay particular attention to this fundamental concept:
    + write kernel code to access the hardware
    + but don't force particular policies on the user.
    + Trade-off between the desire to present the user with as many options as possible</br>
      and the time you have to write the driver
    + Being policy-free is actually a common target for software designers.

+ 这两个函数会休眠
    ```c++
    unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
    unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
    ```
    + 用户态地址 可能还没在 虚拟内存中, 映射虚拟内存是会休眠的
    + 内核态地址 可能还没在 虚拟内存中, 映射虚拟内存是会休眠的

# Linux Driver Development E3 Directories
1. Building and Running Modules      (must) (fini)
2. Debugging Techniques              (must) (fini)
3. Concurrency and Race Conditions   (must) (fini)
    Time, Delays, and Deferred Work  (must) (fini) (core)
    Allocating Memory                (must) (fini) (collect message)
    Communicating with Hardware      (must) (fini) (collect message)
    Memory Mapping and DMA           (must) (fini) (core)
    Interrupt Handling               (must) (fini)
    Data Types in the Kernel         (must) (fini) (useless)
    The Linux Device Model           (need)
4. Char Drivers                      (must) (fini)
   PCI Drivers
   Block Drivers
   USB Drivers
   Network Drivers                   (must) (fini)
   TTY Drivers