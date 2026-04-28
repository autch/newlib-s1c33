/*
 * _sbrk for the EPSON S1C33000 (P/ECE).
 *
 * Memory partition (by convention, not enforced):
 *
 *   [.bss .... kernel pceHeap zone .... _pceheapstart + _pceheapsize)
 *   [_pceheapstart + _pceheapsize ........ newlib sbrk zone ........)
 *   [kernel-reserved high SRAM (work areas, zlib, SYSERRVBUFF, ...) ]
 *
 * Two linker symbols control the layout:
 *
 *   _pceheapstart  = pceAppHead.bss_end (kernel heap base; default
 *                                        __END_DEFAULT_BSS, see piece.ld)
 *   _pceheapsize   = bytes reserved for the kernel-managed pceHeap
 *                                        (default 0x2000 = 8 KB)
 *
 * Newlib's _sbrk hands out memory starting at
 * _pceheapstart + _pceheapsize and grows upward indefinitely.  There
 * is NO upper bound check, on purpose: the kernel reserves some part
 * of high SRAM for its own work areas (zlib buffers, SYSERRVBUFF at
 * 0x13c000, etc.) but the boundary is a runtime value
 * (system_info.sram_end) that we cannot consult at link time.  Apps
 * that grow newlib's heap into the kernel's high-SRAM region corrupt
 * kernel state — the same MMU-less trade-off the platform applies to
 * stack overflow.
 *
 * Alignment: every pointer returned is rounded up to a 4-byte
 * boundary.  S1C33 traps unaligned word/halfword access (manual §2),
 * so this guarantee is required for correctness.
 *
 * Override per-app at link time:
 *
 *     -Wl,--defsym=_pceheapsize=0x4000     (16 KB pceHeap zone, less
 *                                           room for newlib chunks
 *                                           below the kernel's high
 *                                           reserved region)
 *
 * --defsym works for these symbols because they are read as values by
 * sbrk and crt0; no section-size expression touches them, so the
 * lld-specific evaluation-order issue that prevents
 * `--defsym=_heaplen=N` from sizing a .heap section does not apply.
 *
 * Apps with a more exotic layout can replace _sbrk entirely (link
 * order: an object defining _sbrk before -lc wins).
 */

#include <sys/types.h>

extern char _pceheapstart[];
extern char _pceheapsize[];

#define _SBRK_ALIGN 4
#define _SBRK_ALIGN_UP(x) \
  (((uintptr_t) (x) + (_SBRK_ALIGN - 1)) & ~(uintptr_t) (_SBRK_ALIGN - 1))

caddr_t
_sbrk (int incr)
{
  static char *brk = 0;
  char *prev;

  if (brk == 0)
    {
      uintptr_t base = (uintptr_t) _pceheapstart + (uintptr_t) _pceheapsize;
      brk = (char *) _SBRK_ALIGN_UP (base);
    }

  /* Round positive requests up to a multiple of 4 so brk — and the
   * pointer returned next time — stays aligned. */
  if (incr > 0)
    incr = (int) _SBRK_ALIGN_UP (incr);

  prev = brk;
  brk += incr;
  return (caddr_t) prev;
}
