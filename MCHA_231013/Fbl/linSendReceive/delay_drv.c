#include "delay_drv.h"

void delayMS(uint32_t n)
{
    m0_delay_ms(n);
}

void delayUS(uint32_t n)
{
    m0_delay_us(n);
}
