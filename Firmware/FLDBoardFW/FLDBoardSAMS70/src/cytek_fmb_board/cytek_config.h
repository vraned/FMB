/*
 * cytek_config.h
 *
 * Created: 2015/11/16/周一 16:30:07
 *  Author: lenovo
 */ 


#ifndef CYTEK_CONFIG_H_
#define CYTEK_CONFIG_H_

/**********debug port: usart1 or uart0***********/
//#define CYTEK_FMB_CONSOLE_USART1
#define CYTEK_FMB_CONSOLE_UART0


#define CYTEK_QSPI_FLASH_N25Q //do not remove, because the buffer data type is different from sams70 driver, so redefine it


#define CYTEK_FMB_SENSOR_INPUT  /** sensor using IO input mode**/

#define CYTEK_FMB_SENSOR_IRQ 	/** sensor using irq input mode**/

#define CYTEK_FMB_SPI0   /** connect to DAC chips**/

#define CYTEK_FMB_SPI1    /** connect to ADC chips**/

#define CYTEK_FMB_QSPI	/** connect to QSPI flash**/

#define CYTEK_FMB_TWIHS1 

#define CYTEK_FMB_TWIHS2

#define CYTEK_FMB_IO_LED

#define CYTEK_FMB_PUMP

#define CYTEK_FMB_LASER

#define CYTEK_FMB_SOLENOID

#define CYTEK_FMB_MOTOR

#define CYTEK_FMB_POWER_STATE /*power and state led*/
#define CONF_BOARD_CONFIG_MPU_AT_INIT

#endif /* CYTEK_CONFIG_H_ */
