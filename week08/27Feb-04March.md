# Introduction to Posix Threads on Linux<a id="introduction-to-posix-threads-on-linux"></a>

## Linux Thread Programming<a id="linux-thread-programming"></a>

- **What is a thread?**

  - A thread is the smallest unit of execution within a process. It's a sequence of instructions that can be scheduled and executed independently by the operating system's scheduler. Threads within the same process share the same memory space and resources, such as files and sockets, which allows them to communicate and coordinate more efficiently than separate processes.

* **How is threads different with respect to processes?**

  - Threads differ from processes primarily in their level of isolation and resource sharing. While threads within a process share memory and resources, each process has its own memory space and resources. This means that processes are more isolated from each other, but also incur more overhead in terms of communication and resource allocation.

- **When to use separate processes and when to use separate threads?**

  - Separate processes are typically used when you need a high level of isolation between components, such as in the case of independent applications or services. Separate threads, on the other hand, are used when you need more lightweight parallelism within a single application, such as handling multiple client requests concurrently or performing background tasks while keeping the main application responsive.

* **What is kernel level thread? What is user level thread?**

  - Kernel-level threads are managed directly by the operating system kernel, meaning that the kernel is responsible for scheduling and executing them. User-level threads, on the other hand, are managed by a thread library at the user level, which means that the scheduling and execution of these threads are handled by the application itself, without direct intervention from the kernel. Kernel-level threads generally provide better performance and scalability, as the kernel can take advantage of multiprocessor systems and manage threads more efficiently. However, user-level threads can be more flexible and lightweight, as they don't require kernel intervention for thread management.

***


#### POSIX thread (pthread) libraries<a id="posix-thread-pthread-libraries"></a>

#####

#### Thread Basics<a id="thread-basics"></a>

- Thread operations include thread creation, termination, synchronization (joins, blocking), scheduling, data management and process interaction.

- A thread does not maintain a list of created threads, nor does it know the thread that created it.

- All threads within a process share the same address space.

- Threads in the same process share:

  - Process instructions

  - Most data

  - open files (descriptors)

  - signals and signal handlers

  - current working directory

  - User and group id

- Each thread has a unique:

  - Thread ID

  - set of registers, stack pointer

  - stack for local variables, return addresses

  - signal mask

  - priority

  - Return value: errno

- pthread functions return "0" if OK.

***


##### Thread Creation and Termination<a id="thread-creation-and-termination"></a>

Example: pthread1.c

