#ifndef	REBOOT_H
#define	REBOOT_H

#include "peripherals/base.h"
#include "type.h"

// Watchdog and reset controller offsets
#define PM_RSTC_OFFSET      0x0010001C
#define PM_WDOG_OFFSET      0x00100024

// Magic password and full-reset command
#define PM_PASSWORD         0x5A000000
#define PM_RSTC_FULL_RESET  0x00000020

// Convenience pointers
#define PM_RSTC   ((volatile uint32_t *)(BASE + PM_RSTC_OFFSET))
#define PM_WDOG   ((volatile uint32_t *)(BASE + PM_WDOG_OFFSET))

void reboot(void);
#endif  
