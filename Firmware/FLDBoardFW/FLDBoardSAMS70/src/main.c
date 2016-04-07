/*
 * main.c
 *
 *  Created: 11/28/2015
 *  Author: David Florez
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asf.h>
#include "conf_usb.h"
#include "ui.h"
#include "mode_table.h"
#include "moderunner.h"
#include "cytek_ioport.h"
#include "cytek_spi_adc.h"
#include "cytek_spi_dac.h"
#include "cytek_sensor_handler.h"
#include "cytek_motor_driver.h"
#include "cytek_twihs_flow_meter.h"
#include <rtt.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );
static void vSensorMonitorTask( void *pvParameters );

extern SemaphoreHandle_t sPortMutex;
extern SemaphoreHandle_t eStopBinSem;

float CurrentPressSetPoint = 3.0F;
float CorrectedPressSetPoint = 3.0F;

static volatile bool main_b_cdc_enable = false;
mode_table_t ModeTable;
TaskHandle_t MSGTaskHandle = NULL;
TaskHandle_t ModeRTaskHandle = NULL;
TaskHandle_t SensorTaskHandle = NULL;

stepper_motor_t sit_motor;
static void configure_console(void)
 {
	 const usart_serial_options_t uart_serial_options = {
		 .baudrate = CONF_UART_BAUDRATE,
		#ifdef CONF_UART_CHAR_LENGTH
		 .charlength = CONF_UART_CHAR_LENGTH,
		#endif
		 .paritytype = CONF_UART_PARITY,
		#ifdef CONF_UART_STOP_BITS
		 .stopbits = CONF_UART_STOP_BITS,
		#endif
	 };
 
	 /* Configure console UART. */
 #ifdef CYTEK_FMB_CONSOLE_USART1
		  /* Configure console UART. */
		  sysclk_enable_peripheral_clock(ID_USART1);
		  stdio_serial_init(USART1, &uart_serial_options);
#endif
#ifdef CYTEK_FMB_CONSOLE_UART0
		 sysclk_enable_peripheral_clock(ID_UART0);
		 stdio_serial_init(UART0, &uart_serial_options);
	 
#endif
 }


int main(void)
{
    // Initialization tasks
	irq_initialize_vectors();
	cpu_irq_enable();

	// Initialize the sleep manager
	sleepmgr_init();

	sysclk_init();
	board_init();
    //fpu_enable();

	configure_console();

	// Check CPU Clock
	//uint32_t clk = sysclk_get_cpu_hz();


	POWERGOOD_ON;
	STATEGOOD_ON;

	sit_motor.motor_select = MOTOR2;
	sit_motor.tolerance = 25;

	init_accelerate_table(&sit_motor,1150,50,100); //[acc:20, dcc:10]
//	sit_calibration( &sit_motor);


    // Default calibration values

	ModeTable.calibration_values.diff_gain = 273;
	ModeTable.calibration_values.sample_gain = 273;
	ModeTable.calibration_values.diff_ref = 1501;
	ModeTable.calibration_values.sample_ref = 1501;
	ModeTable.calibration_values.diff_offset = 1501;
	ModeTable.calibration_values.sample_offset = 1501;
	
	ModeTable.press_tol = 0.25F;

	ModeTable.ref_temp = 20.0F;
	ModeTable.temp_factor = 0.07F;

	AD5687_init();
	set_diff_xducer_offset(ModeTable.calibration_values.diff_ref << 4);
	set_sample_xducer_offset(ModeTable.calibration_values.sample_ref << 4);
	
	// Initialize Vacuum press ref to 3 PSI
	float temp = get_thermistor_temp();
	float correction = ModeTable.temp_factor * (temp - ModeTable.ref_temp);
	CorrectedPressSetPoint = CurrentPressSetPoint - correction;
	uint32_t prespoint = CorrectedPressSetPoint * ModeTable.calibration_values.diff_gain +
	ModeTable.calibration_values.diff_offset;
	set_pressure_ref(prespoint << 4);
	ModeTable.calibration_table.pressure[0] = CurrentPressSetPoint;

	// Start USB stack to authorize VBus monitoring
	udc_start();

	// Create RTOS tasks
    BaseType_t ret;
    
    // Comms listener Task
    ret = xTaskCreate( vMSGTask, "MSG Task", 2000, NULL, 
                       configMAX_PRIORITIES - 1, &MSGTaskHandle );
    
    if (ret != pdPASS)
    {
        // Should never happen, just for debugging purposes
        while(1);
    }
    
    // Mode Runner Task
    ret = xTaskCreate( vModeRunnerTask, "Mode Run", 2000, NULL,
                       configMAX_PRIORITIES - 3, &ModeRTaskHandle );
    
    if (ret != pdPASS)
    {
        // Should never happen, just for debugging purposes
        while(1);
    }

	// Sensor Monitoring Task
	ret = xTaskCreate( vSensorMonitorTask, "Sensor Mon", 2000, NULL,
	configMAX_PRIORITIES - 4, &SensorTaskHandle );
	
	if (ret != pdPASS)
	{
		// Should never happen, just for debugging purposes
		while(1);
	}

    /* Start the scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was either insufficient FreeRTOS heap memory available for the idle
	and/or timer tasks to be created, or vTaskStartScheduler() was called from
	User mode.  See the memory management section on the FreeRTOS web site for
	more details on the FreeRTOS heap http://www.freertos.org/a00111.html.  The
	mode from which main() is called is set in the C start up code and must be
	a privileged mode (not user mode). */
	
    for( ;; );
}

void main_suspend_action(void)
{
	
}

void main_resume_action(void)
{
	
}

void main_sof_action(void)
{
	if (!main_b_cdc_enable)
		return;
}

#ifdef USB_DEVICE_LPM_SUPPORT
void main_suspend_lpm_action(void)
{
	
}

void main_remotewakeup_lpm_disable(void)
{
	
}

void main_remotewakeup_lpm_enable(void)
{
	
}
#endif

bool main_cdc_enable(uint8_t port)
{
	main_b_cdc_enable = true;
	return true;
}

void main_cdc_disable(uint8_t port)
{
	main_b_cdc_enable = false;
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
	if (b_enable) {
		
	}else{
		
	}
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	/* Remove compiler warning about xFreeHeapSpace being set but never used. */
	( void ) xFreeHeapSpace;
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	
}

#define SHEATH_ALMOST_ALARM 60
#define SHEATH_EMPTY_ALARM   120

// Tasks
void vSensorMonitorTask( void *pvParameters )
{
	TickType_t xDelay;
	char reply[50];
	int sheath = 0;
	int sheath2 = 0;
	bool waste85 = false;
	bool waste99 = false;

	for( ;; )
	{
		//printf("%s\n",__func__);
		// Read Degasser Vacuum Sensor
		if (DEGAS_VACUUM == true)
		{
			PUMP_ON(3);
		}
		else
		{
			PUMP_OFF(3);
		}
		
		// Read Sheath Tank Sensor
		if (SHEATH_LEVEL == true)
		{
			PUMP_ON(1);
			if (sheath < SHEATH_ALMOST_ALARM) sheath++;
			if (sheath2 < SHEATH_EMPTY_ALARM) sheath2++;
		}
		else
		{
			PUMP_OFF(1);
			sheath = 0;
			sheath2 = 0;
		}
		
		if (sheath == SHEATH_ALMOST_ALARM)
		{
			// Send Warning
			xSemaphoreTake(sPortMutex, portMAX_DELAY);
			sprintf(reply, "Sheath Tank: Almost Empty!\n");
			udi_cdc_write_buf((void*)reply, strlen(reply));
			xSemaphoreGive(sPortMutex);
			
			// Don't send the message again
			sheath++;
			
		}

		if (sheath2 == SHEATH_EMPTY_ALARM)
		{
			// Send Error
			xSemaphoreTake(sPortMutex, portMAX_DELAY);
			sprintf(reply, "Sheath Tank: Empty!\n");
			udi_cdc_write_buf((void*)reply, strlen(reply));
			xSemaphoreGive(sPortMutex);

			// Don't send the message again
			sheath2++;

			// Turn Off Plenum Pump
			PUMP_OFF(1);
		}
		
		if ((WASTE_LEVEL_85 == false) && (WASTE_LEVEL_99 == true))
		{
			// Sensor Error
			xSemaphoreTake(sPortMutex, portMAX_DELAY);
			sprintf(reply, "Waste Sensor: ERROR!\n");
			udi_cdc_write_buf((void*)reply, strlen(reply));
			xSemaphoreGive(sPortMutex);
			
			// End current Mode Execution
			xSemaphoreGive(eStopBinSem);
		}
		else
		{
			if (WASTE_LEVEL_99 == true)
			{
				if (waste99 == false)
				{
					xSemaphoreTake(sPortMutex, portMAX_DELAY);
					sprintf(reply, "Waste Tank: FULL!\n");
					udi_cdc_write_buf((void*)reply, strlen(reply));
					xSemaphoreGive(sPortMutex);

					// End current Mode Execution
					xSemaphoreGive(eStopBinSem);
					waste99 = true;
				}
			}

			if (WASTE_LEVEL_85 == true)
			{
				if (waste85 == false)
				{
					xSemaphoreTake(sPortMutex, portMAX_DELAY);
					sprintf(reply, "Waste Tank: Almost Full!\n");
					udi_cdc_write_buf((void*)reply, strlen(reply));
					xSemaphoreGive(sPortMutex);

					waste85 = true;
				}
			}
			else
			{
				waste85 = false;
				waste99 = false;
			}
		}

		// Need to apply Over Vacuum Relief?
//		printf("\n***over vacuum relief part START***\n");
		float diff = get_diff_xducer();
		float setpoint = CorrectedPressSetPoint;
		float tolerance = ModeTable.press_tol;
		bool over_vacuum = (diff - setpoint) > tolerance;
//		printf("***over vacuum relief		diff:%f setpoint:%f tolerance:%f over_vacuum:%d\n",diff, setpoint,tolerance,over_vacuum);
		if ( over_vacuum && (GET_SOLENOID(5) == false) )
		{
			SOLENOID_ON(5);
		}
		else
		{
			SOLENOID_OFF(5);
		}

		// Check it Pressure correction is needed based on temperature changes
		float temp = get_thermistor_temp();
		float correction = ModeTable.temp_factor * (temp - ModeTable.ref_temp);
		CorrectedPressSetPoint = CurrentPressSetPoint - correction;
		uint32_t prespoint = CorrectedPressSetPoint * ModeTable.calibration_values.diff_gain +
		ModeTable.calibration_values.diff_offset;
		set_pressure_ref(prespoint << 4);
//		printf("***over vacuum relief	 	temp:%f temp_factor:%f ref_temp:%f correction:%f\n",temp,ModeTable.temp_factor,ModeTable.ref_temp,correction);
//		printf("***over vacuum relief 	CurrentPressSetPoint:%f CorrectedPressSetPoint:%f  diff_gain:%u 	diff_offset:%u\n",CurrentPressSetPoint,CorrectedPressSetPoint,ModeTable.calibration_values.diff_gain,ModeTable.calibration_values.diff_offset);
//		printf("***over vacuum relief  prespoint:%u\n",prespoint);
//		printf("***over vacuum relief part END***\n\n");
		
		// Flash that Green LED
		GET_STATEGOOD == true ? STATEGOOD_OFF : STATEGOOD_ON;
			
		// Block for 1s.
		xDelay = 1000 / portTICK_PERIOD_MS;
		vTaskDelay(xDelay);

		
		
	}
}