/*
 * cytek_motor_driver.c
 *
 * Created: 2015/12/9/周三 10:47:39
 *  Author: lenovo
 */ 

 #include <asf.h>
 #include <rtt.h>
 #include "cytek_fmb_board.h"
 #include <ioport.h>
 #include "cytek_ioport.h"
 #include "cytek_motor_driver.h"
 #include "cytek_sensor_handler.h"

 /* Kernel includes. */
 #include "FreeRTOS.h"
 #include "task.h"
 #include "timers.h"
 #include "semphr.h"

 extern SemaphoreHandle_t sPortMutex;
 SemaphoreHandle_t spValveMutex;

 stepper_motor_t pinch_valve_stepper;
 
 uint32_t motor1_count = 0;
 uint32_t motor2_count = 0;

 /*flags: the step motor should be accelerated next time*/
 static volatile bool  accel_1 = 1;
 static volatile bool  accel_2 = 1;

 static void motor_step(uint16_t cs, uint32_t steps, uint16_t us_rate);

 void motor_en( uint16_t cs,bool en)
 {
	if(cs == 1){
		if(en)
			MOTOR_ON(1);
		else
			MOTOR_OFF(1);
	}
	if(cs == 2){
		if(en)
			MOTOR_ON(2);
		else
			MOTOR_OFF(2);
	}
 }

/***motor_init: init the pins state **/
 void motor_init(uint16_t cs)
 {
 	if( cs == MOTOR1){
		ioport_as_output_init(MOTOR1_GPIO, IOPORT_DIR_OUTPUT,MOTOR1_INACTIVE_LEVEL);
		ioport_as_output_init(MOTOR1_DIR, IOPORT_DIR_OUTPUT,MOTOR_DIR_DOWN);
		ioport_as_output_init(MOTOR1_PULL, IOPORT_DIR_OUTPUT,MOTOR1_PULL_INACTIVE_LEVEL);
 	}
	if( cs == MOTOR2){
		ioport_as_output_init(MOTOR2_GPIO, IOPORT_DIR_OUTPUT,MOTOR2_INACTIVE_LEVEL);
		ioport_as_output_init(MOTOR2_DIR, IOPORT_DIR_OUTPUT,MOTOR_DIR_UP);
		ioport_as_output_init(MOTOR2_PULL, IOPORT_DIR_OUTPUT,MOTOR2_PULL_INACTIVE_LEVEL);
	}
 }

void motor_run_init(uint16_t cs, bool dir)
{
	if( cs == MOTOR1){
		MOTOR_ON(1);
		delay_us(10);
		ioport_set_pin_level(MOTOR1_DIR,dir);
		delay_us(10);
	}

	if(cs == MOTOR2 ){
		MOTOR_ON(2);
		delay_us(10);
		ioport_set_pin_level(MOTOR2_DIR,dir);
		delay_us(10);
	}
}

static void motor_move( uint16_t cs,uint16_t time_segment)
{
	if( cs == MOTOR1){	
		ioport_set_pin_level(MOTOR1_PULL,MOTOR1_PULL_INACTIVE_LEVEL);
		delay_us(time_segment);
		ioport_set_pin_level(MOTOR1_PULL,MOTOR1_PULL_ACTIVE_LEVEL);
		delay_us(time_segment);
	}
	if( cs == MOTOR2){	
		ioport_set_pin_level(MOTOR2_PULL,MOTOR2_PULL_INACTIVE_LEVEL);
		delay_us(time_segment);
		ioport_set_pin_level(MOTOR2_PULL,MOTOR2_PULL_ACTIVE_LEVEL);
		delay_us(time_segment);
	}
}
 /**motor_run:  run one step
	you must call motor_run() and motor_stop() together for every motor control  phase
**/
 void motor_run(uint16_t cs)
 {
 	static uint16_t times1 = 1000; 
	static uint16_t times2 = 1200; 
	
 	if( cs == MOTOR1){	
		if( accel_1){
			times1 = 1000;
			accel_1 = false ;		
		}

		if(times1 <= 300)
			times1 = 300;
		else
			times1= times1 - 100; //20
		
	 	ioport_set_pin_level(MOTOR1_PULL,MOTOR1_PULL_INACTIVE_LEVEL);
		delay_us(times1);
		ioport_set_pin_level(MOTOR1_PULL,MOTOR1_PULL_ACTIVE_LEVEL);
		delay_us(times1);
 	}
	if (cs == MOTOR2){	
		if( accel_2){
			times2 = 1200;
			accel_2 = false ;		
		}

		if(times2 <= 300)
			times2 = 300;
		else
			times2= times2 - 20;
		
 		ioport_set_pin_level(MOTOR2_PULL,MOTOR2_PULL_INACTIVE_LEVEL);
		delay_us(times2);
		ioport_set_pin_level(MOTOR2_PULL,MOTOR2_PULL_ACTIVE_LEVEL);
		delay_us(times2);
 	}
 }

 void motor_stop(uint16_t cs)
 {
	if( cs == MOTOR1){	
		ioport_set_pin_level(MOTOR1_PULL, MOTOR1_PULL_INACTIVE_LEVEL);
		accel_1 = true;
	}
	if( cs == MOTOR2){	
		ioport_set_pin_level(MOTOR2_PULL,MOTOR2_PULL_INACTIVE_LEVEL);
		accel_2 = true;
	}
 }

void motor_set_dir(uint16_t cs, bool dir)
{
	if( cs == MOTOR1){	
		ioport_set_pin_level(MOTOR1_DIR,dir);
		delay_us(10);
	}
	if( cs == MOTOR2){	
		ioport_set_pin_level(MOTOR2_DIR,dir);
		delay_us(10);
	}
}

void motor_step(uint16_t cs, uint32_t steps, uint16_t us_rate)
 {
	 if( cs == MOTOR1){
		 while (steps)
		 {
			ioport_set_pin_level(MOTOR1_PULL, MOTOR1_PULL_INACTIVE_LEVEL);
			delay_us(us_rate);
			ioport_set_pin_level(MOTOR1_PULL, MOTOR1_PULL_ACTIVE_LEVEL);
			delay_us(us_rate);
			steps--;
		 }
	 }
	 if( cs == MOTOR2){
		 while (steps)
		 {
			 ioport_set_pin_level(MOTOR2_PULL, MOTOR2_PULL_INACTIVE_LEVEL);
			 delay_us(us_rate);
			 ioport_set_pin_level(MOTOR2_PULL, MOTOR2_PULL_ACTIVE_LEVEL);
			 delay_us(us_rate);
			 steps--;
		 }
	 }
 }

 int pinch_valve_stepper_init(void)
 {	
	char reply[50];
	uint32_t n = 0;
	int r = 0;

	spValveMutex = xSemaphoreCreateMutex();
	xSemaphoreTake(spValveMutex, portMAX_DELAY);

	// Take it to Limit
	motor_run_init(MOTOR1, MOTOR_DIR_DOWN); // DIR_DOWN Home -> Limit (Less Flow)
	
	while (!(SAMPLE_STEPPER_HOME && SAMPLE_STEPPER_LIMIT))
	{
		motor_run(MOTOR1);
		if (n++ == 10000)
		{
			// Send Error
			motor_stop(MOTOR1);
			//xSemaphoreTake(sPortMutex, portMAX_DELAY);
			sprintf(reply, "FLD Error: Pinch Valve Limit Sensor Timeout!\n");
			udi_cdc_write_buf((void*)reply, strlen(reply));
			//xSemaphoreGive(sPortMutex);
			//while(1);
			r = -1;
			break;
		}
	}
	motor_stop(MOTOR1);

	// Take it to Home and count
	motor_run_init(MOTOR1, MOTOR_DIR_UP);
	n = 0;
	
	while (SAMPLE_STEPPER_HOME || SAMPLE_STEPPER_LIMIT)
	{
		motor_run(MOTOR1);
		if (n++ == 10000)
		{
			// Send Error
			motor_stop(MOTOR1);
			//xSemaphoreTake(sPortMutex, portMAX_DELAY);
			sprintf(reply, "FLD Error: Pinch Valve Home Sensor Timeout!\n");
			udi_cdc_write_buf((void*)reply, strlen(reply));
			//xSemaphoreGive(sPortMutex);
			//while(1);
			r = -1;
			break;
		}			
	}
	motor_stop(MOTOR1);
	pinch_valve_stepper.travel = n;
	pinch_valve_stepper.pos = 0;

	xSemaphoreGive(spValveMutex);

	return r;
 } 

 void set_flow_rate(int32_t rate)
 {
	xSemaphoreTake(spValveMutex, portMAX_DELAY);

	rate = rate * pinch_valve_stepper.travel / 1000;

	if (rate > pinch_valve_stepper.pos)
	{
		unsigned ncounts = rate - pinch_valve_stepper.pos;
		while (ncounts--)
		{
			motor_run_init(MOTOR1, MOTOR_DIR_DOWN);
			motor_run(MOTOR1);
			pinch_valve_stepper.pos++;
		}
		motor_stop(MOTOR1);
	}
	else
	{
		unsigned ncounts = pinch_valve_stepper.pos - rate;
		while (ncounts--)
		{
			motor_run_init(MOTOR1, MOTOR_DIR_UP);
			motor_run(MOTOR1);
			pinch_valve_stepper.pos--;
		}
		motor_stop(MOTOR1);
	}

	xSemaphoreGive(spValveMutex);
 }

uint32_t acc_timetable[100];

 static void motor_run_mode(uint16_t cs, uint16_t mode, uint16_t step_time_segment)
 {
 /* not completed*/
 #define ACCELERATE 1
 #define NORMAL 2
 #define DECCELERATE 3
	switch(mode){
		case ACCELERATE : 
			motor_move(cs,acc_timetable[step_time_segment]);
			break;
		case NORMAL:
			motor_move(cs,acc_timetable[step_time_segment]);
		case DECCELERATE:
			motor_move(cs,acc_timetable[step_time_segment]);
			break;
		default:
			printf("motor run mode error!\n");
	}
 }

void sit_component_init( stepper_motor_t *sit)
{
	sensor_init(3);
	sensor_init(4);
	sensor_init(5);
	sensor_init(6);

	motor_init(sit->motor_select);
	delay_ms(1);
}
void sit_home_init(stepper_motor_t *sit)
{

	motor_run_init(sit->motor_select,MOTOR_DIR_UP);
	delay_ms(1);
	
	while( !SIT_STEPPER_UPPER){
		motor_run(sit->motor_select);
	}
	motor_stop(sit->motor_select);

	motor_set_dir(sit->motor_select,MOTOR_DIR_DOWN);
	delay_ms(1);
}
 void sit_calibration( stepper_motor_t *sit )
{

	int count_step = 0;
	sit->step_count_d = 0;
	sit->step_count_p = 0;
	sit->step_count_u = 0;
	delay_ms(1000);// wait the power into a steady state
	
	sit_component_init(sit);
	sit_home_init(sit);
	
	if( !MANUAL_TUBE_PRESENCE){
		while( !SIT_STEPPER_PLATE){
			motor_run(sit->motor_select);
			count_step++;
			if(SIT_STEPPER_DOWN)
				sit->step_count_d = count_step;
		}
		motor_stop(sit->motor_select);
		sit->step_count_p = count_step;
		
	}else{
		while( !SIT_STEPPER_DOWN){
			motor_run(sit->motor_select);
			count_step++;
		}
		motor_stop(sit->motor_select);
		sit->step_count_d = count_step;
		
	}

	delay_ms(1);
	sit_home_init(sit);
	sit->step_count_current = 0;
	printf("SIT_DOWN: %d\n SIT_P:%d\n",sit->step_count_d,sit->step_count_p);
}

void init_accelerate_table(stepper_motor_t *sit, int acc_init_value , int acc_delt,int dec_delt)
{ 

#if 1
	int n = 1;
	int size=(sizeof(sit->accelerate_table)/4);
	sit->accelerate_table[0] = acc_init_value;
	printf("%s %d  acc[0]%d\n",__func__,acc_init_value,sit->accelerate_table[0]);
	while( n < size ){
		sit->accelerate_table[n] = sit->accelerate_table[n-1] - acc_delt;
		printf("acc_table[%d]: %d\n",n,sit->accelerate_table[n]);
		n++;
	}

	sit->decelerate_table[0] = sit->accelerate_table[size-1];
	printf("%s  dec[0]%d\n",__func__,sit->decelerate_table[0]);
	n=1;
	size = (sizeof(sit->decelerate_table)/4);
	while( n < size ){
		sit->decelerate_table[n] = sit->decelerate_table[n-1] + dec_delt;
		printf("dec_table[%d]: %d\n",n,sit->decelerate_table[n]);
		n++;
	}
#else
	int n = 1;
	int size=(sizeof(sit->accelerate_table)/4);
	sit->accelerate_table[0] = acc_init_value *0.676 ;
		while( n < size ){
		sit->accelerate_table[n] = sit->accelerate_table[n-1]*(4*n-1)/(4*n+1);
		printf("acc_table[%d]: %d\n",n,sit->accelerate_table[n]);
		n++;
	}

	sit->decelerate_table[0] = sit->accelerate_table[size-1];
	printf("%s  dec[0]%d\n",__func__,sit->decelerate_table[0]);
	n=1;
	size = (sizeof(sit->decelerate_table)/4);
	while( n < size ){
		sit->decelerate_table[n] = sit->decelerate_table[n-1] + dec_delt;
		printf("dec_table[%d]: %d\n",n,sit->decelerate_table[n]);
		n++;
	}
#endif
 
}

void sit_test( stepper_motor_t *sit )
{

		printf("%s  start\n",__func__);
		int n = 0 ;
		int size=(sizeof(sit->accelerate_table)/4);
		while(!SIT_STEPPER_PLATE){
			if( n < size){
				motor_move(sit->motor_select,sit->accelerate_table[n]);
				n++;
			}else{
				motor_move(sit->motor_select,sit->accelerate_table[size-1]);
			}
		}
		printf("%s  end\n",__func__);
}

#if 0
void sit_move(stepper_motor_t *sit, int start_point, int end_point)
{
	//start_point and end_point are the absolute steps based on the SIT_U point
	
	//not use dec part, but there may be take in some mistake after a long time running
	int acc_steps = (sizeof(sit->accelerate_table)/4);
	int acc_counter = 0;
	
	int dec_steps = (sizeof(sit->decelerate_table)/4);
	int dec_counter = 0;
	
	int total_steps = 0;
	
	int normal_counter = 0;
	int normal_steps = 0;

	if( start_point > sit->step_count_p)
		start_point = sit->step_count_p;
	if( start_point < sit->step_count_u)
		start_point = sit->step_count_u;

	if( end_point > sit->step_count_p)
		end_point = sit->step_count_p;
	if( end_point < sit->step_count_u)
		end_point = sit->step_count_u;


	if( start_point > end_point){
		if( ( start_point -end_point) < (acc_steps + dec_steps) )
			return;
		total_steps = start_point-end_point;
		motor_set_dir(sit->motor_select,MOTOR_DIR_UP);
	}else{
		if( (end_point-start_point) < (acc_steps + dec_steps))
			return;
		total_steps = end_point -start_point;
		motor_set_dir(sit->motor_select,MOTOR_DIR_DOWN);
	}

	
	delay_ms(1); //make sure the dir pin is valid before the pull pin

	normal_steps = total_steps - acc_steps;

	while(acc_counter<acc_steps){
		motor_move(sit->motor_select,sit->accelerate_table[acc_counter]);
		acc_counter++;
	} 

	while(normal_counter<normal_steps){
		motor_move(sit->motor_select,sit->accelerate_table[acc_steps-1]);
		normal_counter++;
	}
		

	
}
#else
void sit_move(stepper_motor_t *sit, bool flags, int total_steps)
{	
	 //flags: 1 DOWN ; 0 UP
	int acc_steps = (sizeof(sit->accelerate_table)/4);
	int acc_counter;
	int normal_counter;
	int normal_steps;


	if( flags == true){
		if( ( sit->step_count_current + total_steps) > sit->step_count_p)
			total_steps = sit->step_count_p - sit->step_count_current;
		motor_set_dir(sit->motor_select,MOTOR_DIR_DOWN);
	}else{
		if( ( sit->step_count_current - total_steps) < sit->step_count_u)
			total_steps = sit->step_count_current;
		motor_set_dir(sit->motor_select,MOTOR_DIR_UP);
	}

	delay_ms(1); //make sure the dir pin is valid before the pull pin

	if(total_steps > acc_steps){
		normal_steps = total_steps - acc_steps;
		normal_counter = 0;
		acc_counter = 0;
	
		while(acc_counter < acc_steps){

			motor_move(sit->motor_select,sit->accelerate_table[acc_counter]);
			acc_counter++;
			if(flags)
				sit->step_count_current++;
			else
				sit->step_count_current--;
		} 

		while(normal_counter<normal_steps){

			motor_move(sit->motor_select,sit->accelerate_table[acc_steps-1]);
			normal_counter++;
			if(flags)
				sit->step_count_current++;
			else
				sit->step_count_current--;

		}
	}else{

		acc_counter = 0;
		while(acc_counter < total_steps){
			motor_move(sit->motor_select,sit->accelerate_table[acc_counter]);
			acc_counter++;
			if(flags)
				sit->step_count_current++;
			else
				sit->step_count_current--;

		} 

	}

}	
#endif
 /* example:
		sensor_init(11);
		motor_init();
		motor_run_init(MOTOR1,MOTOR_DIR_DOWN);
		while(1){
			if( sensor_get_level(11))										
				motor_run(MOTOR1);
			else
				motor_stop(MOTOR1);
		}

 */
