/*
 * cytek_ioport.h
 *
 * Created: 2015/11/16/周一 16:48:46
 *  Author: lenovo
 */ 


#ifndef CYTEK_IOPORT_H_
#define CYTEK_IOPORT_H_

#include <ioport.h>
/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
					do {\
						ioport_set_pin_mode(pin, mode);\
						ioport_disable_pin(pin);\
					} while (0)

/**
 * \brief Set input mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 * \param sense Sense for interrupt detection (\ref ioport_sense)
 */
#define ioport_set_pin_input_mode(pin, mode, sense)\
	do {\
		ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);\
		ioport_set_pin_mode(pin, mode);\
		ioport_set_pin_sense_mode(pin, sense);\
	} while(0)
	
#define ioport_as_output_init(pin,mode,sense)\
	do{\
		ioport_set_pin_dir(pin,IOPORT_DIR_OUTPUT);\
		ioport_set_pin_level(pin,sense);\
	}while(0)
	
#define LED_ON(n)  ioport_set_pin_level(LED##n##_GPIO, LED##n##_ACTIVE_LEVEL)
#define LED_OFF(n)  ioport_set_pin_level(LED##n##_GPIO, LED##n##_INACTIVE_LEVEL)


#define PUMP_ON(n)  ioport_set_pin_level(PUMP##n##_GPIO, PUMP##n##_ACTIVE_LEVEL)
#define PUMP_OFF(n)  ioport_set_pin_level(PUMP##n##_GPIO, PUMP##n##_INACTIVE_LEVEL)

#define GET_PUMP(n)  ioport_get_pin_level(PUMP##n##_GPIO)
#define GET_SENSOR(n)  ioport_get_pin_level(SENSOR##n##_GPIO)
#define GET_LASER(n)  ioport_get_pin_level(LASER##n##_GPIO)

#define SAMPLE_STEPPER_LIMIT GET_SENSOR(1)
#define SAMPLE_STEPPER_HOME  GET_SENSOR(2)
#define SIT_STEPPER_UPPER	 GET_SENSOR(3)
#define SIT_STEPPER_DOWN	 GET_SENSOR(4)
#define SIT_STEPPER_PLATE	 GET_SENSOR(5)
#define MANUAL_TUBE_PRESENCE GET_SENSOR(6)
#define SHEATH_LEVEL		 GET_SENSOR(7)
#define WASTE_LEVEL_85	     GET_SENSOR(8)
#define WASTE_LEVEL_99		 GET_SENSOR(9)
#define SIT_DOOR_ILOCK       GET_SENSOR(10)
#define TOP_COVER_ILOCK      GET_SENSOR(11)
#define FLUIDICS_DOOR_ILOCK  GET_SENSOR(12)
#define LOADER_DOOR_ILOCK    GET_SENSOR(13)
#define LASER1_ILOCK         GET_SENSOR(14)
#define LASER2_ILOCK         GET_SENSOR(15)
#define LASER3_ILOCK         GET_SENSOR(16)
#define LASER4_ILOCK         GET_SENSOR(17)
#define DEGAS_VACUUM         GET_SENSOR(18)

#define LASER_ON(n)  ioport_set_pin_level(LASER##n##_GPIO, LASER##n##_ACTIVE_LEVEL)
#define LASER_OFF(n)  ioport_set_pin_level(LASER##n##_GPIO, LASER##n##_INACTIVE_LEVEL)

#define POWERGOOD_ON  ioport_set_pin_level(POWERGOOD_GPIO, POWERGOOD_ACTIVE_LEVEL)
#define POWERGOOD_OFF  ioport_set_pin_level(POWERGOOD_GPIO, POWERGOOD_INACTIVE_LEVEL)


#define STATEGOOD_ON  ioport_set_pin_level(STATEGOOD_GPIO, STATEGOOD_ACTIVE_LEVEL)
#define STATEGOOD_OFF  ioport_set_pin_level(STATEGOOD_GPIO, STATEGOOD_INACTIVE_LEVEL)
#define GET_STATEGOOD  ioport_get_pin_level(STATEGOOD_GPIO)

#define SOLENOID_ON(n)  ioport_set_pin_level(SOLENOID##n##_GPIO, SOLENOID##n##_ACTIVE_LEVEL)
#define SOLENOID_OFF(n)  ioport_set_pin_level(SOLENOID##n##_GPIO, SOLENOID##n##_INACTIVE_LEVEL)

#define GET_SOLENOID(n)  ioport_get_pin_level(SOLENOID##n##_GPIO)


#define MOTOR_ON(n)  ioport_set_pin_level(MOTOR##n##_GPIO, MOTOR##n##_ACTIVE_LEVEL)
#define MOTOR_OFF(n)  ioport_set_pin_level(MOTOR##n##_GPIO, MOTOR##n##_INACTIVE_LEVEL)


#endif /* CYTEK_IOPORT_H_ */