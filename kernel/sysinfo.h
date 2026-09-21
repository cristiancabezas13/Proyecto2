// Structure returned by the sysinfo() system call.
// Shared between kernel and user space.
struct sysinfo {
  uint64 freemem;   // free physical memory, in MB
  uint64 usedpages; // physical pages currently allocated
  uint64 freepages; // physical pages currently free
  uint64 nproc;     // number of processes in RUNNABLE state
};
