/*
 *  moderunner.c
 *
 *  Created: 12/16/2015 
 *  Author: David Florez
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "compiler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "udi_cdc.h"
#include "mode_table.h"
#include "cytek_ioport.h"
#include "cytek_spi_adc.h"
#include "cytek_spi_dac.h"
#include "cytek_sensor_handler.h"
#include "cytek_motor_driver.h"

#include "moderunner.h"

extern const mode_table_t ModeTable;
extern SemaphoreHandle_t sPortMutex;
extern float CurrentPressSetPoint;
extern float CorrectedPressSetPoint;
SemaphoreHandle_t eStopBinSem;

stepper_motor_t sit_motor;

static void sensor_irq_handler(uint32_t a , uint32_t b)
{
	GET_SOLENOID(3) == true ? SOLENOID_OFF(3) : SOLENOID_ON(3);
}

// Tasks
void vModeRunnerTask( void *pvParameters )
{
	uint32_t mailbox;
    	TickType_t xDelay;
    	char reply[50];
	
	eStopBinSem = xSemaphoreCreateBinary();

    	for( ;; )
    	{
        	xTaskNotifyWait( 0x00,  /* Don't clear any notification bits on entry. */
        	ULONG_MAX,              /* Reset the notification value to 0 on exit. */
        	&mailbox,               /* Notified value pass out in mailbox */
        	portMAX_DELAY );        /* Block indefinitely. */
        
       		printf("%s\n",__func__);
        	// Block for 4000ms.
        	//xDelay = 4000 / portTICK_PERIOD_MS;
        	//vTaskDelay(xDelay);
        
        	// Run selected Mode
        	const time_segment_t * pts = ModeTable.mode[mailbox];
        	BaseType_t eStop = pdFALSE;
        
        	for (int n = 0; n < MAX_NUMBER_OF_TIMESEGMENTS; n++, pts++) 
        	{
            		// Set Flowrate to Stepper Pinch Valve
			if (pts->sample != MS_HOLD && pts->sample != MS_DYN)
			{
				uint32_t scounts = ModeTable.calibration_table.sample_flow[pts->sample];
				set_flow_rate(scounts);
			}
            

            		// Send Sit Position to SIT Stepper if required
            		// TODO
            		switch (pts->time) //zrw add
				{
					case T_SITP: sit_move(&sit_motor,1,sit_motor.step_count_p);
						break;
					case T_SITD: sit_move(&sit_motor,sit_motor.step_count_current-sit_motor.step_count_d >0?0:1,abs(sit_motor.step_count_current-sit_motor.step_count_d));
						break;
					case T_SITU: sit_move(&sit_motor,0,sit_motor.step_count_current);
						break;
				//	case T_TUBE: condition_met = MANUAL_TUBE_PRESENCE;
				//		break;
				//	case T_NO_TUBE: condition_met = !MANUAL_TUBE_PRESENCE;
				//		break;
					default: break;
					}
            
            		// Set Valves
            		pts->valves[0] == 0 ? SOLENOID_OFF(1) : SOLENOID_ON(1);    
	            	pts->valves[1] == 0 ? SOLENOID_OFF(2) : SOLENOID_ON(2);
	            	pts->valves[2] == 0 ? SOLENOID_OFF(3) : SOLENOID_ON(3);
	            	pts->valves[3] == 0 ? SOLENOID_OFF(4) : SOLENOID_ON(4);
	            	pts->valves[4] == 0 ? SOLENOID_OFF(5) : SOLENOID_ON(5);
	            	pts->valves[5] == 0 ? SOLENOID_OFF(6) : SOLENOID_ON(6);
            
            		// Set Pressure Reference
            		// Read current Temperature and apply correction
            		if (pts->pumps[0] == 1)
            		{
            			printf("@@@@ Set Pressure Reference START @@@\n");
                		float temp = get_thermistor_temp();
				float correction = ModeTable.temp_factor * (temp - ModeTable.ref_temp);
				CurrentPressSetPoint = ModeTable.calibration_table.pressure[0];
				CorrectedPressSetPoint = CurrentPressSetPoint - correction;
				uint32_t prespoint = CorrectedPressSetPoint * ModeTable.calibration_values.diff_gain +
				ModeTable.calibration_values.diff_offset;
                		set_pressure_ref(prespoint << 4);
		printf("@@@@ Set Pressure Reference temp%f temp_factor%f ref_temp%f \n",temp,ModeTable.temp_factor,ModeTable.ref_temp);
		printf("@@@@ Set Pressure Reference  correction:%f CurrentPressSetPoint%f diff_gain:%u diff_offset:%u\n",correction,CurrentPressSetPoint,ModeTable.calibration_values.diff_gain,ModeTable.calibration_values.diff_offset);
		printf("@@@@ Set Pressure Reference prespoint:%u \n",prespoint);
		printf("@@@@ Set Pressure Reference END @@@\n");				
				// Need to apply Over Vacuum Relief?
				if ( ((get_diff_xducer() - ModeTable.calibration_table.pressure[0]) > ModeTable.press_tol) && (GET_SOLENOID(5) == false) )
				{
					do 
					{
						SOLENOID_ON(5);
						xDelay = 100 / portTICK_PERIOD_MS;
						if ((eStop = xSemaphoreTake(eStopBinSem, xDelay)) == pdTRUE) break;
					} while ((get_diff_xducer() - ModeTable.calibration_table.pressure[0]) > ModeTable.press_tol);
					SOLENOID_OFF(5);
				}
            		}
			else
			{
				// Turn OfF Vacuum Pump
				set_pressure_ref(0);
			}
            
            		// Set Pumps
            		/*
			if ((pts->pumps[1] == 1) && (SHEATH_LEVEL == true))
			{
				// Send Error
				xSemaphoreTake(sPortMutex, portMAX_DELAY);
				sprintf(reply, "Sheath Tank: Empty!\n");
				udi_cdc_write_buf((void*)reply, strlen(reply));
				xSemaphoreGive(sPortMutex);

				// Exit mode execution
				break;
			}
			*/
			pts->pumps[1] == 0 ? PUMP_OFF(1) : PUMP_ON(1);
            		pts->pumps[2] == 0 ? PUMP_OFF(2) : PUMP_ON(2);
            		pts->pumps[3] == 0 ? PUMP_OFF(3) : PUMP_ON(3);
            
            		if (pts->time == T_TOGG)
            		{
                		break;
            		}
            
            		if (pts->time == T_SITU || pts->time == T_SITD || pts->time == T_SITP
											|| pts->time == T_TUBE || pts->time == T_NO_TUBE)
            		{
                		// Wait for sensor, TODO, simulate a delay by now
                		// Block for a couple of seconds
				bool condition_met = false;
				int timeout = 30; // Timeout if condition not met in 15 seconds

				do 
				{
					switch (pts->time)
					{
#if 0
						case T_SITP: condition_met = SIT_STEPPER_PLATE;
						break;
						case T_SITD: condition_met = SIT_STEPPER_DOWN;
						break;
						case T_SITU: condition_met = SIT_STEPPER_UPPER;
						break;
#else // take the sit.tolerance into consider  zrw
						case T_SITP: 
						{
							condition_met = ( SIT_STEPPER_PLATE || (abs(sit_motor.step_count_current - sit_motor.step_count_p) < sit_motor.tolerance) );
							printf("T_SITP abs %d\n",abs(sit_motor.step_count_current - sit_motor.step_count_p) );	
						}
						break;
						case T_SITD: 
						{
							condition_met = (SIT_STEPPER_DOWN || (abs(sit_motor.step_count_current - sit_motor.step_count_d) < sit_motor.tolerance) );
							printf("T_SITD abs %d\n",abs(sit_motor.step_count_current - sit_motor.step_count_d));
						}
						break;
						case T_SITU: condition_met = SIT_STEPPER_UPPER;
						break;
#endif
						case T_TUBE: condition_met = MANUAL_TUBE_PRESENCE;
						break;
						case T_NO_TUBE: condition_met = !MANUAL_TUBE_PRESENCE;
						break;
						default: break;
					}

					if (!condition_met)
					{
					  // Wait 0.5 seconds
					  timeout--;
					  xDelay = 500 / portTICK_PERIOD_MS;
					  if ((eStop = xSemaphoreTake(eStopBinSem, xDelay)) == pdTRUE) break;
					}

					if (timeout == 0)
					{
						switch (pts->time)
						{
							case T_SITP: sprintf(reply, "execMode: SITP Timeout!\n");
							break;
							case T_SITD: sprintf(reply, "execMode: SITD Timeout!\n");
							break;
							case T_SITU: sprintf(reply, "execMode: SITU Timeout!\n");
							break;
							case T_TUBE: sprintf(reply, "execMode: TUBE Timeout!\n");
							break;
							case T_NO_TUBE: sprintf(reply, "execMode: NO_TUBE Timeout!\n");
							break;
							default: break;
						}
						xSemaphoreTake(sPortMutex, portMAX_DELAY);	
						udi_cdc_write_buf((void*)reply, strlen(reply));
						xSemaphoreGive(sPortMutex);
					}
				} while (!condition_met);// && eStop == pdFALSE);
            		}
            		else
            		{
                		xDelay = ((ModeTable.timing_table[pts->time - 1]) * 1000.0f) / portTICK_PERIOD_MS;
                		if ((eStop = xSemaphoreTake(eStopBinSem, xDelay)) == pdTRUE) break;
            		}
        	}
            
        	if (eStop == pdFALSE)
		{
			xSemaphoreTake(sPortMutex, portMAX_DELAY);
			
			if (ModeTable.nModes == mailbox + 1)
			{
				sprintf(reply, "execModeEnd: STOP Finished\n");
			}
			else
			{
				sprintf(reply, "execModeEnd: %d  Finished\n", (int)mailbox + 1);
			}
			
			udi_cdc_write_buf((void*)reply, strlen(reply));
        
			xSemaphoreGive(sPortMutex);    
         	}
    }
}            