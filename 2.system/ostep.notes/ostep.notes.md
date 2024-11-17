[TOC]
https://pages.cs.wisc.edu/~remzi/OSTEP/
比 "内核设计与实现" 还要简单的概念入门

+ separation of policy and mechanism:
    + a common design paradigm is to separate high-level policies from their low-level mechanisms
    + mechanism: answers of **how** questions (e.g. how to perform a context switch ?)
    + policy: answers of **which** questions (e.g. which process should the run right now?)

### CPU
#### Process
#### Process API
#### Direct Execution
#### CPU Scheduling
#### Multi-level Feedback
#### Lottery Scheduling
#### Multi-CPU Scheduling

### MEM
#### Address Spaces
#### Memory API
#### Addresss Translation
#### Segmentation
#### Free Space Management
#### Introduction to Paging
#### Translation Lookaside Buffers
#### Advanced Page Tables
#### Swapping Mechanisms
#### Swapping Policies
#### Complete VM Systems

### Concurrency
#### Thread
#### Thread API
#### Lock
#### Locked Data Structures
#### Condition Variable
#### Semaphores
#### Concurrency Bugs
#### Event-based Concurrency

### Persistence
#### I/O Devices
#### Hard Disk Drives
#### Redundant Disk Arrays (RAID)
#### Files and Directories
#### File System Implementation
#### Fast File System (FFS)
#### Fsck and Journaling
#### Log-Structured File System (LFS)
#### Flash-based SSDs
#### Data Integrity and Protection
#### Distributed Systems
#### Network File System (NFS)
#### Andrew File System (AFS)