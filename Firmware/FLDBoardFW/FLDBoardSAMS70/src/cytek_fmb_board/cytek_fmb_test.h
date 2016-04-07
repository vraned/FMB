/*
 * cytek_fmb_test.h
 *
 * Created: 2015/12/4/周五 9:26:17
 *  Author: lenovo
 */ 


#ifndef CYTEK_FMB_TEST_H_
#define CYTEK_FMB_TEST_H_


enum test_mode{
	TEST_ALL = 0,
	TEST_IO,
	TEST_SOLENOID,
	TEST_FLASH,
	TEST_ADC,
	TEST_DAC,
	TEST_MOTOR,
	TEST_SENSOR,
	TEST_PUMP,
	TEST_FLOW_METER,
	TEST_USB,
};

enum test_state{
	PASSED = 0,
	ERROR_IO,
	ERROR_FLASH,
	ERROR_ADC,
	ERROR_DAC,
	ERROR_MOTOR,
	ERROR_SENSOR,
	ERROR_PUMP,
	ERROR_FLOW_METER,
	ERROR_USB,
};


enum test_state cytek_fmb_test_mode(enum test_mode mode);

#endif /* CYTEK_FMB_TEST_H_ */