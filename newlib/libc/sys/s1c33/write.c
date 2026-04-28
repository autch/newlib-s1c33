/*
 * Default _write for the EPSON S1C33000 (P/ECE).
 *
 * Defined as weak so applications can replace it with their own
 * implementation: semihosting, serial output via the expansion port,
 * an in-memory log buffer, or a P/ECE LCD trace.  The default
 * pretends every write succeeds so printf does not loop or fail.
 */

#include <sys/types.h>

__attribute__ ((weak))
int
_write (int fd, const char *buf, int len)
{
  (void) fd;
  (void) buf;
  return len;
}
