// Test program for the trace() syscall.
//
// Usage: trace <syscall_name> <command> [args...]
//
// Enables kernel-level tracing for <syscall_name> (e.g. "sys_kill",
// "sys_write") and then execs into <command>, so that any invocation
// of the traced syscall made by that command prints PID, syscall
// name, return value and a few RISC-V registers to the console.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "Usage: %s <syscall_name> <command> [args...]\n", argv[0]);
    exit(1);
  }

  if (trace(argv[1]) < 0) {
    fprintf(2, "%s: unknown syscall '%s'\n", argv[0], argv[1]);
    exit(1);
  }

  exec(argv[2], &argv[2]);

  // exec only returns on failure.
  fprintf(2, "%s: exec %s failed\n", argv[0], argv[2]);
  exit(1);
}
