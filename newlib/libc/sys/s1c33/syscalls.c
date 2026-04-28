/*
 * POSIX syscall stubs for the EPSON S1C33000 (P/ECE).
 *
 * The P/ECE platform has no filesystem, no fork/exec, no signals,
 * and no threads.  Almost every syscall returns -1 with errno set
 * to ENOSYS or a more specific code where conventional.  A few
 * stubs return benign values so newlib's stdio path does not
 * mistakenly think a real fd is being addressed:
 *   _close  returns 0   (printf cleanup is a no-op)
 *   _lseek  returns 0   (printf may probe end-of-file for width)
 *   _fstat  reports S_IFCHR for any fd (terminal-like; no buffering)
 *   _isatty returns 1 for fd 0/1/2
 *
 * Compiled with -DMISSING_SYSCALL_NAMES (set in configure.host) so
 * symbol names use the underscore-prefixed variants.
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>

int
_read (int fd, char *buf, int len)
{
  (void) fd;
  (void) buf;
  (void) len;
  errno = ENOSYS;
  return -1;
}

int
_close (int fd)
{
  (void) fd;
  return 0;
}

int
_lseek (int fd, int offset, int whence)
{
  (void) fd;
  (void) offset;
  (void) whence;
  return 0;
}

int
_open (const char *path, int flags, int mode)
{
  (void) path;
  (void) flags;
  (void) mode;
  errno = ENOSYS;
  return -1;
}

int
_fstat (int fd, struct stat *st)
{
  (void) fd;
  st->st_mode = S_IFCHR;
  return 0;
}

int
_stat (const char *path, struct stat *st)
{
  (void) path;
  (void) st;
  errno = ENOENT;
  return -1;
}

int
_isatty (int fd)
{
  return (fd >= 0 && fd <= 2);
}

int
_link (const char *old, const char *new)
{
  (void) old;
  (void) new;
  errno = EMLINK;
  return -1;
}

int
_unlink (const char *path)
{
  (void) path;
  errno = ENOENT;
  return -1;
}

int
_kill (int pid, int sig)
{
  (void) pid;
  (void) sig;
  errno = EINVAL;
  return -1;
}

int
_getpid (void)
{
  return 1;
}

int
_fork (void)
{
  errno = EAGAIN;
  return -1;
}

int
_execve (const char *path, char *const argv[], char *const envp[])
{
  (void) path;
  (void) argv;
  (void) envp;
  errno = ENOMEM;
  return -1;
}

int
_wait (int *status)
{
  (void) status;
  errno = ECHILD;
  return -1;
}

clock_t
_times (struct tms *buf)
{
  (void) buf;
  return (clock_t) -1;
}

int
_gettimeofday (struct timeval *tv, void *tz)
{
  (void) tv;
  (void) tz;
  errno = ENOSYS;
  return -1;
}
