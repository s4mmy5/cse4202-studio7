# Studio 7: Tracing the Linux Kernel

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

