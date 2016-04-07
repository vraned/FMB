/*
 * cytek_fmb_test.c
 *
 * Created: 2015/12/4/周五 9:23:31
 *  Author: lenovo
 */ 

 
 #include <asf.h>
 #include <stdio.h>
 #include <qspi.h>
 #include "n25q_interface.h"
 #include "cytek_time.h"
 #include "cytek_fmb_test.h"
 #include "cytek_ioport.h"
 #include "cytek_spi_adc.h"
 #include "cytek_spi_dac.h"
 #include "cytek_sensor_handler.h"
 #include "cytek_twihs_flow_meter.h"
 #include "cytek_qspi_flash_driver.h"

#ifdef TEST_DEBUG
#define dbg_printf printf
#else
#define dbg_printf
#endif

const char *test_string[10]={
	 "test mode  passed!",
	 "test mode IO error!",
	 "test mode FLASH error!",
	 "test mode ADC error!",
	 "test mode DAC error!",
	 "test mode MOTOR error!",
	 "test mode SENSOR error!",
	 "test mode PUMP error!",
	 "test mode FLOW METER error!",
	 "test mode USB error!",
 };

static void led_test(void)
{
	int cycle = 10;
	printf("TEST MODE: %s\n",__func__);
	while(cycle-->0){
		LED_ON(1);
		LED_ON(2);
		LED_ON(3);
		LED_ON(4);
		LED_ON(5);
		LED_ON(6);
	//	LED_ON(7);  //this pin has a problem
		LED_ON(8);
		LED_ON(9);
		LED_ON(10);
		delay_ms(500);
		LED_OFF(1);
		LED_OFF(2);
		LED_OFF(3);
		LED_OFF(4);
		LED_OFF(5);
		LED_OFF(6);
	//	LED_OFF(7);      //this pin has a problem
		LED_OFF(8);
		LED_OFF(9);
		LED_OFF(10);	
		delay_ms(500);
	}
}

 static void solenoid_test(void)
 {
 	int cycle = 10;
	printf("TEST MODE: %s\n",__func__);
	while(cycle-->0){
		SOLENOID_ON(1);
		SOLENOID_ON(2);
		SOLENOID_ON(3);
		SOLENOID_ON(4);
		SOLENOID_ON(5);
		SOLENOID_ON(6);
		delay_ms(5000);
		SOLENOID_OFF(1);
		SOLENOID_OFF(2);
		SOLENOID_OFF(3);
		SOLENOID_OFF(4);
		SOLENOID_OFF(5);
		SOLENOID_OFF(6);
		delay_ms(5000);
	}
 }

static  void power_good(void)
{
	POWERGOOD_ON;
}

static   void state_good(void)
{
	STATEGOOD_ON;
}


static  enum cytek_fmb_state flash_test(void)
 {
	enum cytek_fmb_state state;

//BUFFER_SIZE is limited by stack size,so change the stack size before change BUFFER_SIZE	
#define BUFFER_SIZE  5120   
#define FLASH_ADDRESS   0xaeffff
	
		uint32_t i = 0; 
		uint8_t buffer[BUFFER_SIZE];
		flash_init();
		
		for(; i < BUFFER_SIZE; i++)
			buffer[i] = i%256;
		
		state = flash_write(FLASH_ADDRESS,buffer,BUFFER_SIZE);
		
		while( state != FLASH_OK )
			dbg_printf(" Flash write error	 %d\n",state);
		
		for(; i < BUFFER_SIZE; i++)
			buffer[i] = 0;
		
		flash_read(FLASH_ADDRESS,buffer,BUFFER_SIZE);

		for(i=0;i<BUFFER_SIZE;i++)
			dbg_printf("%d ", buffer[i]);

 }

 static  enum test_state adc_test(void)
 {
 	printf("TEST MODE: %s\n",__func__);
	uint16_t adc_value;

	adc_value =get_sample_xducer();
	printf("read sample pressure: 0x%x\n",adc_value);
	 adc_value = get_diff_xducer();
	 printf("read vacuum pressure: 0x%x\n",adc_value);	 
	 adc_value = get_thermistor_temp();
	printf("read temperature: 0x%x       %d\n",adc_value,adc_value);
	 return PASSED;

 }

  static enum test_state dac_test(void)
 {
 	 AD5687_init();
	  /*form a high_middle_low wave*/
	  printf("TEST MODE: %s\n",__func__);
	  set_diff_xducer_offset(0xffff);
	  set_sample_xducer_offset(0xffff);
	  set_pressure_ref(0xffff);
	  delay_ms(100);
	  set_diff_xducer_offset(0x4444);
	  set_sample_xducer_offset(0x4444);
	  set_pressure_ref(0x4444);
	  delay_ms(100);
	  set_diff_xducer_offset(0x0);
	  set_sample_xducer_offset(0x0);
	  set_pressure_ref(0x0);
	  delay_ms(100);
	  return PASSED;

 }


 static  enum test_state motor_test(void)
 {
	/*you can find the example in cytek_motor_driver.c*/
	printf("TEST MODE: %s\n",__func__);
	return PASSED;
 }

static   enum test_state sensor_test(void)
 {
	/*not complete yet*/
#if 1
			sensor_init(1);
			sensor_init(2);
			sensor_init(3);
			sensor_init(4);
			sensor_init(5);
			sensor_init(6);
			sensor_init(7);
			sensor_init(8);
			sensor_init(9);
			sensor_init(10);
			sensor_init(11);
			sensor_init(12);
			sensor_init(13);
			sensor_init(14);
			sensor_init(15);
			sensor_init(16);
			sensor_init(17);
			sensor_init(18);
			sensor_init(19);
			sensor_init(20);
	
			while(1){
				if(sensor_get_level(1))
					printf("1\n");
				if(sensor_get_level(2))
					printf("2\n");
				if(sensor_get_level(3))
					printf("3\n");
				if(sensor_get_level(4))
					printf("4\n");
				if(sensor_get_level(5))
					printf("5\n");
				if(sensor_get_level(6))
					printf("6\n");
				if(sensor_get_level(7))
					printf("7\n");
				if(sensor_get_level(8))
					printf("8\n");
				if(sensor_get_level(9))
					printf("9\n");
				if(sensor_get_level(10))
					printf("10\n");
				if(sensor_get_level(11))
					printf("11\n");
				if(sensor_get_level(12))
					printf("12\n");
				if(sensor_get_level(13))
					printf("13\n");
				if(sensor_get_level(14))
					printf("14\n");
				if(sensor_get_level(15))
					printf("15\n");
				if(sensor_get_level(16))
					printf("16\n");
				if(sensor_get_level(17))
					printf("17\n");
				if(sensor_get_level(18))
					printf("18\n");
				if(sensor_get_level(19))
					printf("19\n");
				if(sensor_get_level(20))
					printf("20\n");
			}
	
#else
	printf("TEST MODE: %s\n",__func__);
	/* it seems that the total irq source is up to 7 */
	config_sensor_irq_handler1();
	config_sensor_irq_handler2();
	config_sensor_irq_handler3();
	config_sensor_irq_handler4();
	config_sensor_irq_handler5();
	config_sensor_irq_handler6();
	config_sensor_irq_handler7();
//	config_sensor_irq_handler8();
//	config_sensor_irq_handler9();
	config_sensor_irq_handler10();
	config_sensor_irq_handler11();
	config_sensor_irq_handler12();
	config_sensor_irq_handler13();
	config_sensor_irq_handler14();
	config_sensor_irq_handler15();
	config_sensor_irq_handler16();
	config_sensor_irq_handler17();
	config_sensor_irq_handler18();
	config_sensor_irq_handler19();
	config_sensor_irq_handler20();

#endif
	return PASSED;

 }

static   enum test_state pump_test(void)
 {
	printf("TEST MODE: %s\n",__func__);
	PUMP_ON(1);
	PUMP_ON(2);
	PUMP_ON(3);
	delay_ms(5000);
	PUMP_OFF(1);
	PUMP_OFF(2);
	PUMP_OFF(3);	
	delay_ms(5000);
	return PASSED;

 }


static   enum test_state flow_meter_test(void)
 {
	/*not complete yet*/
	uint32_t result;
	printf("TEST MODE: %s\n",__func__);

#if 0
	result = flow_meter_init();
	if ( result != TWIHS_SUCCESS) {
		printf("-E-\tTWI master initialization failed : %d.\r",result);
		while (1);
	}
#endif

	result = flow_meter_temp_measure(); 
	if ( result == -1)
		printf("flow_meter_temp_measure failed\r");
	else
		printf("flow_meter_temp_measure:%d\r",result);
	delay_ms(300);

	result = flow_meter_vdd_measure(); 
	if ( result == -1)
		printf("flow_meter_vdd_measure failed\r");
	else
		printf("flow_meter_vdd_measure:%d\r",result);
	delay_ms(300);

	result = flow_meter_flow_measure();
	if ( result == -1) 
		printf("flow_meter_flow_measuret failed\r");
	else
		printf("flow_meter_flow_measuret:%d\r",result);
	
	result = flow_meter_get_measure_unit(); 
	if ( result == -1) 
		printf("flow_meter_get_measure_unit failed\r");
	else
		printf("flow_meter_get_measure_unit:%d\r",result);
	
	return PASSED;

 }

 static   enum test_state usb_test(void)
 {
	 /*not complete yet*/
	 uint8_t usb_buffer[10];
	 int usb_char;
	 printf("TEST MODE: %s\n",__func__);
	 udc_start();
#if 0
	while(1){
		 /*read a char from host, this function leads to poll*/
		 usb_char = udi_cdc_getc();
		 printf("usb get a char:	%c      %d\n",usb_char,usb_char);
		 /*send a char to host*/
		 udi_cdc_putc(usb_char);
		 delay_s(1);
	}
#else
	while(1){
		 /*read some char from host to buffer, this function leads to poll*/
		 udi_cdc_read_buf(usb_buffer,10);
		 printf("read usb buf:%s\n",usb_buffer);
		 /*send some chars foorm buffer to host*/
		 udi_cdc_write_buf(usb_buffer,10);
	}
#endif
	 return PASSED;
 }

  
 enum test_state cytek_fmb_test_mode(enum test_mode mode)
{
	enum test_state ret;
	
	power_good();
	
	if(mode){
		switch(mode){
			case TEST_IO:
				led_test();
				break;
			case TEST_SOLENOID:
				solenoid_test();
				break;
			case TEST_FLASH:
				ret = flash_test();
				break;
			case TEST_ADC:
				ret = adc_test();
				break;
			case TEST_DAC:
				ret = dac_test();
				break;
			case TEST_MOTOR:
				ret = motor_test();
				break;
			case TEST_SENSOR:
				ret = sensor_test();
				break;
			case TEST_PUMP:
				ret = pump_test();
				break;
			case TEST_FLOW_METER:
				ret = flow_meter_test();
				break;
			case TEST_USB:
				ret = usb_test();
				break;

			default:
				break;
			}// end of switch
	}else{
		led_test();
		solenoid_test();
		
		ret = flash_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
		
		ret = adc_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
	
		ret = dac_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
	
		ret = motor_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
	
		ret = sensor_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
	
		ret = pump_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}
	
		ret = flow_meter_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}

		ret = usb_test();
		if(!ret){
			printf("%s\n",test_string[ret]);
			return ret;
		}

	} //if
	
	printf("%s\n",test_string[ret]);
	return ret;
}

 
