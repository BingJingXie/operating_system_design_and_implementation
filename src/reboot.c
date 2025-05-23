#include "peripherals/reboot.h"


void reboot(void) {
    // Set watchdog timer to short timeout (e.g., 10 ticks ≈ a few ms)
    *PM_WDOG = PM_PASSWORD | 10;

    // Request a full reset
    *PM_RSTC = PM_PASSWORD | PM_RSTC_FULL_RESET;

    // Wait forever for the reset to occur
    while (1) { }
}
