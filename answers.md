# Studio 7: Tracing the Linux Kernel

[link](https://classes.engineering.wustl.edu/cse4202/studios/07_kernel_tracing.html)

## Exercises

1. Contributors:
   - Jonathan Rodriguez Gomez

2. Source code:

```
int main(void) { return 0;}
```

3. strace output:

```
set_tid_address(0xffff8e9f5010)         = 195
set_robust_list(0xffff8e9f5020, 24)     = 0
```

These calls setup up the thread data, starting with the thread ID (i.e. the tid).  set_robust_list updates the robust list, or futex list with a new head pointer, this list is kept to inform other threads holding mutexes of the death of a thread in the same group.

4. sing syscall:

The syscall used here is write().

5. The main difference in these programs is the size of their dynamic memory allocations.

```

30,32c30,32
< getrandom("\x13\x88\x7a\xf9\x30\x79\xc0\xa9", 8, GRND_NONBLOCK) = 8
< brk(NULL)                               = 0xaaaae4fbb000
< brk(0xaaaae4fdc000)                     = 0xaaaae4fdc000
---
> getrandom("\xc3\x91\xd8\x07\x25\xa3\x4f\x46", 8, GRND_NONBLOCK) = 8
> brk(NULL)                               = 0xaaab04f71000
> brk(0xaaab04f92000)                     = 0xaaab04f92000
34c34,36
< brk(0xaaaae5003000)                     = 0xaaaae5003000
---
> mmap(NULL, 720896, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xffffae2e0000
> mmap(NULL, 720896, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xffffae230000
> mmap(NULL, 720896, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xffffae180000

```

Here we can see that the dense300 call used mmap instead of brk to set the dynamic memory segments. This is very much intended behavior in libc, since using mmap for bigger allocations reduced program fragmentation and avoid heap segment limits altogether.

6. Malloc mmap threshold:

```
27,30c27,31
< munmap(0xffff8b834000, 10871)           = 0
< getrandom("\x4a\xa5\xd9\x12\xea\x44\xca\x4a", 8, GRND_NONBLOCK) = 8
< brk(NULL)                               = 0xaaaaecca6000
< brk(0xaaaaeccc7000)                     = 0xaaaaeccc7000
---
> munmap(0xffff80004000, 10871)           = 0
> getrandom("\xca\x8b\x51\xa9\xc7\x9e\x5b\x15", 8, GRND_NONBLOCK) = 8
> brk(NULL)                               = 0xaaab1af37000
> brk(0xaaab1af58000)                     = 0xaaab1af58000
> mmap(NULL, 1003520, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xffff7fd0b000
```

When dynamically allocating 100 bytes the malloc function used the brk syscall to grow the heap segment since this is a relatively small block. However, when allocating 1 billion bytes malloc switches to using mmap so avoid fragmentation when freeing large memory blocks.

7. Install trace-cmd:

```
sammy@mikey:~ $ time sudo apt install trace-cmd
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
trace-cmd is already the newest version (2.9.1-1).
The following package was automatically installed and is no longer required:
  libfuse2
Use 'sudo apt autoremove' to remove it.
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.

real    0m3.499s
user    0m3.211s
sys     0m0.208s
```

8. sched_switch events per cpu:

```
CPU ID | COUNT
[000]  |    40 
[001]  |    12 
[002]  |   163
[003]  |    14 
```
