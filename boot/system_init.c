#include "device.h"
#include "clock.h"

void SystemInit(void)
{
    /* Initialize the system clock tree to 168 MHz */
    clock_init();
}
