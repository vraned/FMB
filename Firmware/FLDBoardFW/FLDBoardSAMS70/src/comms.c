/*
 * comms.c
 *
 *  Created: 12/2/2015 
 *  Author: David Florez
 */ 

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "compiler.h"
#include "udi_cdc.h"
#include "ui.h"
#include "comms.h"
#include "mode_table.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "msgcodes.h"
#include "cytek_ioport.h"
#include "cytek_spi_adc.h"
#include "cytek_spi_dac.h"
#include "cytek_sensor_handler.h"
#include "cytek_motor_driver.h"
#include "cytek_twihs_flow_meter.h"

// Private prototypes
static uint8_t receive_mode_table(void);

// Externs and Globals
extern mode_table_t ModeTable;
extern TaskHandle_t MSGTaskHandle;
extern TaskHandle_t ModeRTaskHandle;
extern stepper_motor_t pinch_valve_stepper;
extern SemaphoreHandle_t spValveMutex;
extern SemaphoreHandle_t eStopBinSem;
extern float CurrentPressSetPoint;
extern float CorrectedPressSetPoint;
SemaphoreHandle_t sPortMutex;

bool isHWInit = false;
uint32_t SensirionScale;

extern stepper_motor_t sit_motor;

void usb_rx_notify(uint8_t port)
{
    UNUSED(port);
    
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    xTaskNotifyFromISR( MSGTaskHandle, 0xDEADBEEF, eSetValueWithoutOverwrite, &xHigherPriorityTaskWoken );    
    
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

// Tasks
void vMSGTask( void *pvParameters )
{
    uint32_t mailbox;
    uint8_t msg;
    volatile iram_size_t n = 0;
    volatile iram_size_t r = 0;
    
    sPortMutex = xSemaphoreCreateMutex();
    
    for( ;; )
    {
        // For debugging purpoSensirio
        if (r != 0)
        {
            n = 0;
        }
        
        xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
        ULONG_MAX,         /* Reset the notification value to 0 on exit. */
        &mailbox,          /* Notified value pass out in mailbox */
        portMAX_DELAY );   /* Block indefinitely. */
        
        // Message from host arrived
        n = udi_cdc_get_nb_received_data();
        while (n == 0)
        {
            n = udi_cdc_get_nb_received_data();
        }
        r = udi_cdc_read_buf((void*)&msg, 1);
        
        xSemaphoreTake(sPortMutex, portMAX_DELAY);
        
        // Decode message and act on it
        switch (msg)
        {
            case MSG_LOAD_MODE_TABLE:
            {
                // Receive Mode Table
                uint8_t t = receive_mode_table();
                if (t == 0xAB)
                {
                    const char * reply = "ModeTable received by Target. Wait for HW Init\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    
					if (isHWInit == false)
					{
						// DF
						flow_meter_init();
						SensirionScale = flow_meter_get_scale_factor();

						
						if (pinch_valve_stepper_init() == 0)
						{
							const char * reply2 = "HW Initialization Done\n";
							udi_cdc_write_buf((void*)reply2, strlen(reply2));
							isHWInit = true;
						}						
					}					

                    // Clear pending notifications
                    xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                    ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                    &mailbox,          /* Notified value pass out in mailbox */
                    portMAX_DELAY );   /* Block indefinitely. */
                }
                else
                {
                    while(1); // Should never happen
                }
            }
            break;
            
            case MSG_EXEC_MODE:
            {   
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                r = udi_cdc_read_buf((void*)&msg, 1);
                sprintf(reply, "execModeBegin: %d\n", msg + 1);
                udi_cdc_write_buf((void*)reply, strlen(reply));
                
                // Wake Up Mode 
                xTaskNotify(ModeRTaskHandle, msg, eSetValueWithoutOverwrite);
                
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
			case MSG_E_STOP:
			{
				// Emergency Stop!!!

				// End current Mode Execution
				xSemaphoreGive(eStopBinSem);
				
				// Turn Lasers Off
				LASER_OFF(1);
				LASER_OFF(2);
				LASER_OFF(3);

				// Exec eStop Mode 
				xTaskNotify(ModeRTaskHandle, ModeTable.nModes - 1, eSetValueWithoutOverwrite);

				const char * reply = "eStop: Received\n";
				udi_cdc_write_buf((void*)reply, strlen(reply));
				if (r != 0) continue;
			}
			break;

            case MSG_DUMMY_MESSAGE:
            {
                // Do nothing, duh!
                const char * reply = "Don't be such a dummy!\n";
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;
            
            case MSG_SET_CAL_VALUES:
            {
                const int args = 6;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                { 
                    n = udi_cdc_get_nb_received_data();
                }
                
                if (n != (args * sizeof(float)))
                {
                    // Wrong number of args
                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
                    const char * reply = "Incorrect number of arguments\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                }
                else 
                {
                    r = udi_cdc_read_buf((void*)argsval, n);
                    const char * reply = "SetCalValues received\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    ModeTable.calibration_values.diff_gain      = (uint32_t)argsval[0];
                    ModeTable.calibration_values.diff_ref    = (uint32_t)argsval[1];
                    ModeTable.calibration_values.sample_gain    = (uint32_t)argsval[2];
                    ModeTable.calibration_values.sample_ref  = (uint32_t)argsval[3];
                    ModeTable.calibration_values.diff_offset     = (uint32_t)argsval[4];
                    ModeTable.calibration_values.sample_offset   = (uint32_t)argsval[5];
                    
                    set_diff_xducer_offset(ModeTable.calibration_values.diff_ref << 4);
                    set_sample_xducer_offset(ModeTable.calibration_values.sample_ref << 4);
                    
                }    
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */                
            }
            break;
            
            case MSG_GET_CAL_VALUES:
            {
                char reply[100];
                sprintf(reply, "Calibration values: %u, %u, %u, %u, %u, %u\n",
                                ModeTable.calibration_values.diff_gain,
                                ModeTable.calibration_values.diff_ref,
                                ModeTable.calibration_values.sample_gain,
                                ModeTable.calibration_values.sample_ref,
                                ModeTable.calibration_values.diff_offset,
                                ModeTable.calibration_values.sample_offset);               
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;
            
            case MSG_SET_VALVE:
            {
                const int args = 2;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                if (n != (args * sizeof(float)))
                {
                    // Wrong number of args
                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
                    const char * reply = "Incorrect number of arguments\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                }
                else
                {
                    char reply[100];
                    r = udi_cdc_read_buf((void*)argsval, n);
                    sprintf(reply, "setValve: %u %s\n", (unsigned)argsval[0], 
                                    (unsigned)argsval[1] == 0 ? "Off" : "On");
                    
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    
                    if ((unsigned)argsval[1] == 0)
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: SOLENOID_OFF(1); break;
                            case 2: SOLENOID_OFF(2); break;
                            case 3: SOLENOID_OFF(3); break;
                            case 4: SOLENOID_OFF(4); break;
                            case 5: SOLENOID_OFF(5); break;
                            case 6: SOLENOID_OFF(6); break;
                        }
                    }
                    else
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: SOLENOID_ON(1); break;
                            case 2: SOLENOID_ON(2); break;
                            case 3: SOLENOID_ON(3); break;
                            case 4: SOLENOID_ON(4); break;
                            case 5: SOLENOID_ON(5); break;
                            case 6: SOLENOID_ON(6); break;
                        }
                    }
                    
                }
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_PUMP:
            {
                const int args = 2;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                if (n != (args * sizeof(float)))
                {
                    // Wrong number of args
                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
                    const char * reply = "Incorrect number of arguments\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                }
                else
                {
                    char reply[100];
                    r = udi_cdc_read_buf((void*)argsval, n);
                    sprintf(reply, "setPump: %u %s\n", (unsigned)argsval[0],
                    (unsigned)argsval[1] == 0 ? "Off" : "On");
                    
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    
                    if ((unsigned)argsval[1] == 0)
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: PUMP_OFF(1); break;
                            case 2: PUMP_OFF(2); break;
                            case 3: PUMP_OFF(3); break;
                        }
                    }
                    else
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: PUMP_ON(1); break;
                            case 2: PUMP_ON(2); break;
                            case 3: PUMP_ON(3); break;
                        }
                    }
                    
                }
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_GET_VALVE:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                bool valveon = false;
                r = udi_cdc_read_buf((void*)argsval, n);
                switch ((unsigned)argsval[0])
                {
                    case 1: valveon = GET_SOLENOID(1); break;
                    case 2: valveon = GET_SOLENOID(2); break;
                    case 3: valveon = GET_SOLENOID(3); break;
                    case 4: valveon = GET_SOLENOID(4); break;
                    case 5: valveon = GET_SOLENOID(5); break;
                    case 6: valveon = GET_SOLENOID(6); break;
                }
                    
                sprintf(reply, "getValve: %u %s\n", (unsigned)argsval[0],
                valveon == true ? "On" : "Off");
                    
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
			case MSG_INCREASE_FLOW_RATE:
            {
                const int args = 1;
                float argsval[args];
				char reply[100];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                r = udi_cdc_read_buf((void*)argsval, n);
                
				unsigned ncounts = ((unsigned)pinch_valve_stepper.travel / (unsigned)argsval[0]) + 1;   
				
				xSemaphoreTake(spValveMutex, portMAX_DELAY);
				while (ncounts-- != 0)
				{
					motor_run_init(MOTOR1, MOTOR_DIR_UP);
					motor_run(MOTOR1);
					pinch_valve_stepper.pos--;
				}
                motor_stop(MOTOR1);
				xSemaphoreGive(spValveMutex);
				
				sprintf(reply, "getFlowRate: %d\n", (int)pinch_valve_stepper.pos);
				udi_cdc_write_buf((void*)reply, strlen(reply));
				    
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;

			case MSG_DECREASE_FLOW_RATE:
			{
				const int args = 1;
				float argsval[args];
				char reply[100];
				
				n = udi_cdc_get_nb_received_data();
				while (n == 0)
				{
					n = udi_cdc_get_nb_received_data();
				}
				
				r = udi_cdc_read_buf((void*)argsval, n);
				
				unsigned ncounts = ((unsigned)pinch_valve_stepper.travel / (unsigned)argsval[0]) + 1;
				
				xSemaphoreTake(spValveMutex, portMAX_DELAY);
				while (ncounts-- != 0)
				{
					motor_run_init(MOTOR1, MOTOR_DIR_DOWN);
					motor_run(MOTOR1);
					pinch_valve_stepper.pos++;
				}
				motor_stop(MOTOR1);
				xSemaphoreGive(spValveMutex);

				sprintf(reply, "getFlowRate: %d\n", (int)pinch_valve_stepper.pos);
				udi_cdc_write_buf((void*)reply, strlen(reply));

				if (r != 0) continue;
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
			}
			break;

			case MSG_SET_FLOW_RATE:
			{
				const int args = 1;
				float argsval[args];
				char reply[100];
				
				n = udi_cdc_get_nb_received_data();
				while (n == 0)
				{
					n = udi_cdc_get_nb_received_data();
				}
				
				r = udi_cdc_read_buf((void*)argsval, n);
				
				set_flow_rate(argsval[0]);
				
				sprintf(reply, "getFlowRate: %d\n", (int)pinch_valve_stepper.pos);				
				udi_cdc_write_buf((void*)reply, strlen(reply));
				
				if (r != 0) continue;
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
			}
			break;

            case MSG_GET_PUMP:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                bool pumpon = false;
                r = udi_cdc_read_buf((void*)argsval, n);
                switch ((unsigned)argsval[0])
                {
                    case 1: pumpon = GET_PUMP(1); break;
                    case 2: pumpon = GET_PUMP(2); break;
                    case 3: pumpon = GET_PUMP(3); break;
                }
                
                sprintf(reply, "getPump: %u %s\n", (unsigned)argsval[0],
                pumpon == true ? "On" : "Off");
                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_GET_SENSOR:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                bool sensoron = false;
                r = udi_cdc_read_buf((void*)argsval, n);
                switch ((unsigned)argsval[0])
                {
                    case 1: sensoron = GET_SENSOR(1); break;
                    case 2: sensoron = GET_SENSOR(2); break;
                    case 3: sensoron = GET_SENSOR(3); break;
                    case 4: sensoron = GET_SENSOR(4); break;
                    case 5: sensoron = GET_SENSOR(5); break;
                    case 6: sensoron = GET_SENSOR(6); break;
                    case 7: sensoron = GET_SENSOR(7); break;
                    case 8: sensoron = GET_SENSOR(8); break;
                    case 9: sensoron = GET_SENSOR(9); break;
                    case 10: sensoron = GET_SENSOR(10); break;
                    case 11: sensoron = GET_SENSOR(11); break;
                    case 12: sensoron = GET_SENSOR(12); break;
                    case 13: sensoron = GET_SENSOR(13); break;
                    case 14: sensoron = GET_SENSOR(14); break;
                    case 15: sensoron = GET_SENSOR(15); break;
                    case 16: sensoron = GET_SENSOR(16); break;
                    case 17: sensoron = GET_SENSOR(17); break;
                    case 18: sensoron = GET_SENSOR(18); break;
                    case 19: sensoron = GET_SENSOR(19); break;
                    case 20: sensoron = GET_SENSOR(20); break;
                }
                
                sprintf(reply, "getSensor: %u %s\n", (unsigned)argsval[0],
                sensoron == true ? "On" : "Off");
                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_LASER:
            {
                const int args = 2;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                if (n != (args * sizeof(float)))
                {
                    // Wrong number of args
                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
                    const char * reply = "Incorrect number of arguments\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                }
                else
                {
                    char reply[100];
                    r = udi_cdc_read_buf((void*)argsval, n);
                    sprintf(reply, "setLaser: %u %s\n", (unsigned)argsval[0],
                    (unsigned)argsval[1] == 0 ? "Off" : "On");
                    
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    
                    if ((unsigned)argsval[1] == 0)
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: LASER_OFF(1); break;
                            case 2: LASER_OFF(2); break;
                            case 3: LASER_OFF(3); break;
                        }
                    }
                    else
                    {
                        switch ((unsigned)argsval[0])
                        {
                            case 1: LASER_ON(1); break;
                            case 2: LASER_ON(2); break;
                            case 3: LASER_ON(3); break;
                        }
                    }
                    
                }
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_GET_LASER:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                bool laseron = false;
                r = udi_cdc_read_buf((void*)argsval, n);
                switch ((unsigned)argsval[0])
                {
                    case 1: laseron = GET_LASER(1); break;
                    case 2: laseron = GET_LASER(2); break;
                    case 3: laseron = GET_LASER(3); break;
                }
                
                sprintf(reply, "getLaser: %u %s\n", (unsigned)argsval[0],
                laseron == true ? "On" : "Off");
                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_LASER_DELAY:
            {
                const int args = 2;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                if (n != (args * sizeof(float)))
                {
                    // Wrong number of args
                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
                    const char * reply = "Incorrect number of arguments\n";
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                }
                else
                {
                    char reply[100];
                    r = udi_cdc_read_buf((void*)argsval, n);
                    sprintf(reply, "setLaserDelay: %u %.2f\n", (unsigned)argsval[0],
                    argsval[1]);
                    
                    udi_cdc_write_buf((void*)reply, strlen(reply));
                    
                    ModeTable.laser_delay[(unsigned)argsval[0] - 1] = argsval[1];
                    
                }
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_TEMP_FACTOR:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                r = udi_cdc_read_buf((void*)argsval, n);
                sprintf(reply, "setTempFactor: %.2f\n", argsval[0]);
                    
                udi_cdc_write_buf((void*)reply, strlen(reply));
                    
                ModeTable.temp_factor = argsval[0];
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_GET_TEMP_FACTOR:
            {
                char reply[100];
                sprintf(reply, "getTempFactor: %.2f\n", ModeTable.temp_factor);
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;
            
			case MSG_GET_REF_TEMP:
			{
				char reply[100];
				sprintf(reply, "getRefTemp: %.2f\n", ModeTable.ref_temp);
				udi_cdc_write_buf((void*)reply, strlen(reply));
				if (r != 0) continue;
			}
			break;

            case MSG_GET_TEMP:
            {
                char reply[100];
                sprintf(reply, "getTemp: %.2f\n", get_thermistor_temp());
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;
            
            case MSG_GET_PRESS:
            {
                char reply[100];
                sprintf(reply, "getPres: %.2f\n", get_diff_xducer());
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;

			case MSG_READ_SENSIRION:
			{
				char reply[100];
				int16_t flow = flow_meter_flow_measure();
				sprintf(reply, "readSensirion: %.2f\n", (float)flow / SensirionScale);
				udi_cdc_write_buf((void*)reply, strlen(reply));
				if (r != 0) continue;
			}
			break;

			case MSG_GET_SAMPLE:
			{
				char reply[100];
				sprintf(reply, "getSample: %.2f\n", get_sample_xducer());
				udi_cdc_write_buf((void*)reply, strlen(reply));
				if (r != 0) continue;
			}
			break;

            case MSG_GET_PRESS_TOL:
            {
                char reply[100];
                sprintf(reply, "getPresTol: %.2f\n", ModeTable.press_tol);
                udi_cdc_write_buf((void*)reply, strlen(reply));
                if (r != 0) continue;
            }
            break;
            case MSG_SET_PRESS:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                r = udi_cdc_read_buf((void*)argsval, n);
                sprintf(reply, "setPress: %.2f\n", argsval[0]);                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                
                // Read current Temperature and apply correction
                CurrentPressSetPoint = argsval[0];
				uint32_t prespoint = CurrentPressSetPoint * ModeTable.calibration_values.diff_gain + 
				ModeTable.calibration_values.diff_offset;
                set_pressure_ref(prespoint << 4);
                
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_REF_TEMP:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                r = udi_cdc_read_buf((void*)argsval, n);
                sprintf(reply, "setRefTemp: %.2f\n", argsval[0]);
                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                
                ModeTable.ref_temp = argsval[0];
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;
            
            case MSG_SET_PRESS_TOL:
            {
                const int args = 1;
                float argsval[args];
                
                n = udi_cdc_get_nb_received_data();
                while (n == 0)
                {
                    n = udi_cdc_get_nb_received_data();
                }
                
                char reply[100];
                r = udi_cdc_read_buf((void*)argsval, n);
                sprintf(reply, "setPresTol: %.2f\n", argsval[0]);
                
                udi_cdc_write_buf((void*)reply, strlen(reply));
                
                ModeTable.press_tol = argsval[0];
                if (r != 0) continue;
                // Clear pending notifications
                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
                &mailbox,          /* Notified value pass out in mailbox */
                portMAX_DELAY );   /* Block indefinitely. */
            }
            break;

		case MSG_GET_SIT_POS:
		{
		                char reply[100];
		                sprintf(reply, "getSIT pos: %d, %d, %d\n",sit_motor.step_count_u,sit_motor.step_count_d,sit_motor.step_count_p);               
		                udi_cdc_write_buf((void*)reply, strlen(reply));
		                if (r != 0) continue;
#if 0
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
#endif
		}
		break;
		case MSG_SET_SIT_TOL:
		{
				const int args = 1;
				float argsval[args];
				char reply[50];
				
				n = udi_cdc_get_nb_received_data();
				while (n == 0)
				{
					n = udi_cdc_get_nb_received_data();
				}

				r = udi_cdc_read_buf((void*)argsval, n);
				sit_motor.tolerance = (uint32_t)argsval[0];
				sprintf(reply, "setSITtolerance:%d\n",sit_motor.tolerance);				
				udi_cdc_write_buf((void*)reply, strlen(reply));
				
		                if (r != 0) continue;
		                // Clear pending notifications
		                xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
		                ULONG_MAX,         /* Reset the notification value to 0 on exit. */
		                &mailbox,          /* Notified value pass out in mailbox */
		                portMAX_DELAY );   /* Block indefinite*/
		}
		break;
		case MSG_GET_SIT_TOL:
		{
		                char reply[50];
		                sprintf(reply, "getSIT tolerance: %d\n",sit_motor.tolerance);               
		                udi_cdc_write_buf((void*)reply, strlen(reply));
		                if (r != 0) continue;
#if 0
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
#endif
		}
		break;
		case MSG_SIT_CAL:
		{
				sit_calibration( &sit_motor);
		                char reply[50];
		                sprintf(reply, "calSIT done\n");               
		                udi_cdc_write_buf((void*)reply, strlen(reply));				
		                if (r != 0) continue;
#if 0
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
#endif
		}
		break;

		case MSG_SIT_MOVE:
		{
	                const int args = 2;
	                float argsval[args];
	                
	                n = udi_cdc_get_nb_received_data();
	                while (n == 0)
	                { 
	                    n = udi_cdc_get_nb_received_data();
	                }
	                
	                if (n != (args * sizeof(float)))
	                {
	                    // Wrong number of args
	                    udi_cdc_read_buf((void*)argsval, n); // Flush the buffer
	                    const char * reply = "Incorrect number of arguments\n";
	                    udi_cdc_write_buf((void*)reply, strlen(reply));
	                }else {
	                    r = udi_cdc_read_buf((void*)argsval, n);
	                    bool flags   = (int)argsval[0];
	                    int total_steps= (int)argsval[1];
	                    sit_move(&sit_motor,flags,total_steps);
 			    const char * reply = "Run finish\n";
	                    udi_cdc_write_buf((void*)reply, strlen(reply));
	                }   			
		                if (r != 0) continue;
				// Clear pending notifications
				xTaskNotifyWait( 0x00,              /* Don't clear any notification bits on entry. */
				ULONG_MAX,         /* Reset the notification value to 0 on exit. */
				&mailbox,          /* Notified value pass out in mailbox */
				portMAX_DELAY );   /* Block indefinitely. */
		}
		break;
		
            default:
            while(1); // Should never happen
            break;
        }
       xSemaphoreGive(sPortMutex); 
    }
}

uint8_t receive_mode_table(void)
{
    iram_size_t n;
    iram_size_t r;
    
    // Receive Mode Table
    // Timing Table First
    
    n = udi_cdc_get_nb_received_data();
    while (n == 0)
    {
        n = udi_cdc_get_nb_received_data();
    }
    
    r = udi_cdc_read_buf((void*)ModeTable.timing_table, n);
    
    // Calibration Table
    n = udi_cdc_get_nb_received_data();
    while (n == 0)
    {
        n = udi_cdc_get_nb_received_data();
    }
    
    r = udi_cdc_read_buf((void*)ModeTable.calibration_table.pressure, sizeof(float));
    
    n = udi_cdc_get_nb_received_data();
    while (n == 0)
    {
        n = udi_cdc_get_nb_received_data();
    }
    
    r = udi_cdc_read_buf((void*)ModeTable.calibration_table.sample_flow, 3 * sizeof(uint32_t));
    
    n = udi_cdc_get_nb_received_data();
    while (n == 0)
    {
        n = udi_cdc_get_nb_received_data();
    }
    
    r = udi_cdc_read_buf((void*)ModeTable.calibration_table.sit, 3 * sizeof(uint32_t));
    
    // Receive Modes
    int m = 0;
    do
    {
        int t = 0;
        do
        {
            n = udi_cdc_get_nb_received_data();
            while (n == 0)
            {
                n = udi_cdc_get_nb_received_data();
            }
            
            udi_cdc_read_buf((void*)ModeTable.mode[m][t].valves, NUMBER_OF_VALVES);
            
            n = udi_cdc_get_nb_received_data();
            while (n == 0)
            {
                n = udi_cdc_get_nb_received_data();
            }
            
            udi_cdc_read_buf((void*)ModeTable.mode[m][t].pumps, NUMBER_OF_PUMPS);
            
            n = udi_cdc_get_nb_received_data();
            while (n == 0)
            {
                n = udi_cdc_get_nb_received_data();
            }
            
            udi_cdc_read_buf((void*)&(ModeTable.mode[m][t].sample), 1);
            
            n = udi_cdc_get_nb_received_data();
            while (n == 0)
            {
                n = udi_cdc_get_nb_received_data();
            }
            
            r = udi_cdc_read_buf((void*)&(ModeTable.mode[m][t].time), 1);
            
            t++;
        } while (ModeTable.mode[m][t-1].time != T_TOGG);
        
        m++;
    } while(n == 1);

	ModeTable.nModes = m;

    // Flush out termination byte
    r = udi_cdc_getc();
    
    
    return r;
}