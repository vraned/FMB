/*
 * cytek_sensor_handler.h
 *
 * Created: 2015/12/2/周三 9:43:56
 *  Author: lenovo
 */ 


#ifndef CYTEK_SENSOR_HANDLER_H_
#define CYTEK_SENSOR_HANDLER_H_

#include <asf.h>
#include <asf.h>
#include <spi.h>
#include <ioport.h>
#include <cytek_time.h>
#include <cytek_spi_adc.h>
#include <delay.h>


/*********************************sensor poll mode*************************************/
#define sensor_init(N)		ioport_set_pin_input_mode(SENSOR##N##_GPIO,SENSOR##N##_FLAGS,SENSOR##N##_SENSE)
#define sensor_get_level(N) 	ioport_get_pin_level(SENSOR##N##_GPIO)

/**Example:	sensor input 

		sensor_init(17);
		while(true){
	
				state = sensor_get_level(17);/////ioport_get_pin_level(SENSOR17_GPIO);
					printf("sensor17  : %d\n",state);
				mdelay(1000);
		}
		
**/
/*********************************sensor poll mode end*************************************/

 void config_sensor_irq_handler1(void);
 void config_sensor_irq_handler2(void);
 void config_sensor_irq_handler3(void);
 void config_sensor_irq_handler4(void);
 void config_sensor_irq_handler5(void);
 void config_sensor_irq_handler6(void);
 void config_sensor_irq_handler7(void);
 void config_sensor_irq_handler8(void);
 void config_sensor_irq_handler9(void);
 void config_sensor_irq_handler10(void);
 void config_sensor_irq_handler11(void);
 void config_sensor_irq_handler12(void);
 void config_sensor_irq_handler13(void);
 void config_sensor_irq_handler14(void);
 void config_sensor_irq_handler15(void);
 void config_sensor_irq_handler16(void);
 void config_sensor_irq_handler17(void);
 void config_sensor_irq_handler18(void);
 void config_sensor_irq_handler19(void);
 void config_sensor_irq_handler20(void);



#endif /* CYTEK_SENSOR_HANDLER_H_ */
