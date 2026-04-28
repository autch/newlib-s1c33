/*
 * _exit for the EPSON S1C33000 (P/ECE).
 *
 * P/ECE applications never "exit" in the conventional sense.  The
 * kernel drives the lifecycle through pceAppInit / pceAppProc /
 * pceAppExit callbacks and reclaims the application by resetting,
 * not by tearing down a process.  An app that reaches _exit (e.g.
 * via abort()) has nowhere to return to, so we spin.
 */

void
_exit (int status)
{
  (void) status;
  for (;;)
    __asm__ volatile ("nop");
}
