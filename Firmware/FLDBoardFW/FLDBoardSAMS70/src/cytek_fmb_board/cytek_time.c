/*
 * cytek_time.c
 *
 * Created: 2015/11/11/周三 13:58:50
 *  Author: lenovo
 */ 

#include "cytek_time.h"
#include "delay.h"

//static volatile uint32_t g_ul_ms_ticks = 0;

// DF: We have FreeRTOS now, we need the SysTick Handler for OS tasks
//void SysTick_Handler(void)
//{
//	g_ul_ms_ticks++;
//}


void mdelay(volatile uint32_t ul_dly_ticks)
{
#if 0
	uint32_t ul_cur_ticks = ul_dly_ticks * 1000;

	ul_cur_ticks = g_ul_ms_ticks;
	while ((g_ul_ms_ticks - ul_cur_ticks) < ul_dly_ticks);
    // Before the SysTick was setup at CPU_HZ/1000, so we simulate active wait now
#else
    delay_us(ul_dly_ticks*1000);
#endif
}
