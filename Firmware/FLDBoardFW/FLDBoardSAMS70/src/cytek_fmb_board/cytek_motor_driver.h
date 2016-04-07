/*
 * cytek_motor_driver.h
 *
 * Created: 2015/12/9/周三 10:48:02
 *  Author: lenovo
 */ 


#ifndef CYTEK_MOTOR_DRIVER_H_
#define CYTEK_MOTOR_DRIVER_H_

typedef struct  
{
	bool reverse;
	uint32_t travel;
	int32_t pos;
	
	int accelerate_table[20];
	//int accelerate_table[7];
	int decelerate_table[20];
	int step_count_u;
	int step_count_d;
	int step_count_p;
	int step_count_current;
	int tolerance;
	uint16_t motor_select;
} stepper_motor_t;

void motor_init(uint16_t cs);
void motor_en( uint16_t cs,bool en);
void motor_run_init(uint16_t cs, bool dir);
void motor_run(uint16_t cs);
void motor_stop(uint16_t cs);
void motor_set_dir(uint16_t cs, bool dir);
int  pinch_valve_stepper_init(void);
void set_flow_rate(int32_t rate);

void sit_home_init( stepper_motor_t *sit);
void sit_calibration( stepper_motor_t *sit );
void sit_component_init( stepper_motor_t *sit);

void init_accelerate_table(stepper_motor_t *sit, int acc_init_value , int acc_delt,int dec_delt);
void sit_test( stepper_motor_t *sit );
void sit_move(stepper_motor_t *sit, bool flags, int total_steps);
#endif /* CYTEK_MOTOR_DRIVER_H_ */