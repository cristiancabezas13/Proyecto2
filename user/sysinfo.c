// Test program for the sysinfo() syscall.
//
// Usage: sysinfo
//
// Queries the kernel for current memory and process statistics and
// prints them to stdout.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(void)
{
  struct sysinfo info;

  if (sysinfo(&info) < 0) {
    fprintf(2, "sysinfo: failed to retrieve system information\n");
    exit(1);
  }

  printf("Free Memory: %d MB\n", (int)info.freemem);
  printf("Used Pages: %d\n", (int)info.usedpages);
  printf("Available Pages: %d\n", (int)info.freepages);
  printf("Runnable Processes: %d\n", (int)info.nproc);

  exit(0);
}
