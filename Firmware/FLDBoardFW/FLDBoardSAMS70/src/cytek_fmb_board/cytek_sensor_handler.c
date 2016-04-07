/*
 * cytek_sensor_handler.c
 *
 * Created: 2015/12/2/周三 9:38:31
 *  Author: lenovo
 */ 

#include "cytek_sensor_handler.h"
#include <cytek_config.h>
#include <cytek_ioport.h>
//#include "cytek_motor_driver.h"

/*********************************sensor interrupt mode*************************************/
#define SENSOR1_ID ID_PIOC
#define SENSOR1_PIO PIOC
#define SENSOR1_MASK  PIO_PC20
#define IRQ_PRIOR_PIO 0
#define SENSOR1_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler1(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler1(void)
{
	pmc_enable_periph_clk(SENSOR1_ID);
	pio_set_debounce_filter(SENSOR1_PIO, SENSOR1_MASK, 10);
	
	pio_handler_set(SENSOR1_PIO, SENSOR1_ID,
			SENSOR1_MASK, SENSOR1_ATTR, sensor_irq_handler1);
	NVIC_EnableIRQ((IRQn_Type) SENSOR1_ID);
	pio_handler_set_priority(SENSOR1_PIO,
			(IRQn_Type) SENSOR1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR1_PIO, SENSOR1_MASK);

}

#define SENSOR2_ID ID_PIOC
#define SENSOR2_PIO PIOC
#define SENSOR2_MASK  PIO_PC18
#define IRQ_PRIOR_PIO 0
#define SENSOR2_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler2(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler2(void)
{
	pmc_enable_periph_clk(SENSOR2_ID);
	pio_set_debounce_filter(SENSOR2_PIO, SENSOR2_MASK, 10);
	
	pio_handler_set(SENSOR2_PIO, SENSOR2_ID,
			SENSOR2_MASK, SENSOR2_ATTR, sensor_irq_handler2);
	NVIC_EnableIRQ((IRQn_Type) SENSOR2_ID);
	pio_handler_set_priority(SENSOR2_PIO,
			(IRQn_Type) SENSOR2_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR2_PIO, SENSOR2_MASK);

}

#define SENSOR3_ID ID_PIOC
#define SENSOR3_PIO PIOC
#define SENSOR3_MASK  PIO_PC22
#define IRQ_PRIOR_PIO 0
#define SENSOR3_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler3(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler3(void)
{
	pmc_enable_periph_clk(SENSOR3_ID);
	pio_set_debounce_filter(SENSOR3_PIO, SENSOR3_MASK, 10);
	
	pio_handler_set(SENSOR3_PIO, SENSOR3_ID,
			SENSOR3_MASK, SENSOR3_ATTR, sensor_irq_handler3);
	NVIC_EnableIRQ((IRQn_Type) SENSOR3_ID);
	pio_handler_set_priority(SENSOR3_PIO,
			(IRQn_Type) SENSOR3_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR3_PIO, SENSOR3_MASK);

}



#define SENSOR4_ID ID_PIOC
#define SENSOR4_PIO PIOC
#define SENSOR4_MASK  PIO_PC17
#define IRQ_PRIOR_PIO 0
#define SENSOR4_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler4(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler4(void)
{
	pmc_enable_periph_clk(SENSOR4_ID);
	pio_set_debounce_filter(SENSOR4_PIO, SENSOR4_MASK, 10);
	
	pio_handler_set(SENSOR4_PIO, SENSOR4_ID,
			SENSOR4_MASK, SENSOR4_ATTR, sensor_irq_handler4);
	NVIC_EnableIRQ((IRQn_Type) SENSOR4_ID);
	pio_handler_set_priority(SENSOR4_PIO,
			(IRQn_Type) SENSOR4_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR4_PIO, SENSOR4_MASK);

}

#define SENSOR5_ID ID_PIOC
#define SENSOR5_PIO PIOC
#define SENSOR5_MASK  PIO_PC14
#define IRQ_PRIOR_PIO 0
#define SENSOR5_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler5(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler5(void)
{
	pmc_enable_periph_clk(SENSOR5_ID);
	pio_set_debounce_filter(SENSOR5_PIO, SENSOR5_MASK, 10);
	
	pio_handler_set(SENSOR5_PIO, SENSOR5_ID,
			SENSOR5_MASK, SENSOR5_ATTR, sensor_irq_handler5);
	NVIC_EnableIRQ((IRQn_Type) SENSOR5_ID);
	pio_handler_set_priority(SENSOR5_PIO,
			(IRQn_Type) SENSOR5_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR5_PIO, SENSOR5_MASK);

}


#define SENSOR6_ID ID_PIOC
#define SENSOR6_PIO PIOC
#define SENSOR6_MASK  PIO_PC10
#define IRQ_PRIOR_PIO 0
#define SENSOR6_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler6(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler6(void)
{
	pmc_enable_periph_clk(SENSOR6_ID);
	pio_set_debounce_filter(SENSOR6_PIO, SENSOR6_MASK, 10);
	
	pio_handler_set(SENSOR6_PIO, SENSOR6_ID,
			SENSOR6_MASK, SENSOR6_ATTR, sensor_irq_handler6);
	NVIC_EnableIRQ((IRQn_Type) SENSOR6_ID);
	pio_handler_set_priority(SENSOR6_PIO,
			(IRQn_Type) SENSOR6_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR6_PIO, SENSOR6_MASK);

}


#define SENSOR7_ID ID_PIOC
#define SENSOR7_PIO PIOC
#define SENSOR7_MASK  PIO_PC8
#define IRQ_PRIOR_PIO 0
#define SENSOR7_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler7(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler7(void)
{
	pmc_enable_periph_clk(SENSOR7_ID);
	pio_set_debounce_filter(SENSOR7_PIO, SENSOR7_MASK, 10);
	
	pio_handler_set(SENSOR7_PIO, SENSOR7_ID,
			SENSOR7_MASK, SENSOR7_ATTR, sensor_irq_handler7);
	NVIC_EnableIRQ((IRQn_Type) SENSOR7_ID);
	pio_handler_set_priority(SENSOR7_PIO,
			(IRQn_Type) SENSOR7_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR7_PIO, SENSOR7_MASK);

}


#define SENSOR8_ID ID_PIOC
#define SENSOR8_PIO PIOC
#define SENSOR8_MASK  PIO_PC5
#define IRQ_PRIOR_PIO 0
#define SENSOR8_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler8(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler8(void)
{
	pmc_enable_periph_clk(SENSOR8_ID);
	pio_set_debounce_filter(SENSOR8_PIO, SENSOR8_MASK, 10);
	
	pio_handler_set(SENSOR8_PIO, SENSOR8_ID,
			SENSOR8_MASK, SENSOR8_ATTR, sensor_irq_handler8);
	NVIC_EnableIRQ((IRQn_Type) SENSOR8_ID);
	pio_handler_set_priority(SENSOR8_PIO,
			(IRQn_Type) SENSOR8_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR8_PIO, SENSOR8_MASK);

}

#define SENSOR9_ID ID_PIOC
#define SENSOR9_PIO PIOC
#define SENSOR9_MASK  PIO_PC4
#define IRQ_PRIOR_PIO 0
#define SENSOR9_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler9(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler9(void)
{
	pmc_enable_periph_clk(SENSOR9_ID);
	pio_set_debounce_filter(SENSOR9_PIO, SENSOR9_MASK, 10);
	
	pio_handler_set(SENSOR9_PIO, SENSOR9_ID,
			SENSOR9_MASK, SENSOR9_ATTR, sensor_irq_handler9);
	NVIC_EnableIRQ((IRQn_Type) SENSOR9_ID);
	pio_handler_set_priority(SENSOR9_PIO,
			(IRQn_Type) SENSOR9_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR9_PIO, SENSOR9_MASK);

}

#define SENSOR10_ID ID_PIOC
#define SENSOR10_PIO PIOC
#define SENSOR10_MASK  PIO_PC2
#define IRQ_PRIOR_PIO 0
#define SENSOR10_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler10(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler10(void)
{
	pmc_enable_periph_clk(SENSOR10_ID);
	pio_set_debounce_filter(SENSOR10_PIO, SENSOR10_MASK, 10);
	
	pio_handler_set(SENSOR10_PIO, SENSOR10_ID,
			SENSOR10_MASK, SENSOR10_ATTR, sensor_irq_handler10);
	NVIC_EnableIRQ((IRQn_Type) SENSOR10_ID);
	pio_handler_set_priority(SENSOR10_PIO,
			(IRQn_Type) SENSOR10_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR10_PIO, SENSOR10_MASK);

}

#define SENSOR11_ID ID_PIOC
#define SENSOR11_PIO PIOC
#define SENSOR11_MASK  PIO_PC19
#define IRQ_PRIOR_PIO 0
#define SENSOR11_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler11(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler11(void)
{
	pmc_enable_periph_clk(SENSOR11_ID);
	pio_set_debounce_filter(SENSOR11_PIO, SENSOR11_MASK, 10);
	
	pio_handler_set(SENSOR11_PIO, SENSOR11_ID,
			SENSOR11_MASK, SENSOR11_ATTR, sensor_irq_handler11);
	NVIC_EnableIRQ((IRQn_Type) SENSOR11_ID);
	pio_handler_set_priority(SENSOR11_PIO,
			(IRQn_Type) SENSOR11_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR11_PIO, SENSOR11_MASK);

}


#define SENSOR12_ID ID_PIOC
#define SENSOR12_PIO PIOC
#define SENSOR12_MASK  PIO_PC21
#define IRQ_PRIOR_PIO 0
#define SENSOR12_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler12(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler12(void)
{
	pmc_enable_periph_clk(SENSOR12_ID);
	pio_set_debounce_filter(SENSOR12_PIO, SENSOR12_MASK, 10);
	
	pio_handler_set(SENSOR12_PIO, SENSOR12_ID,
			SENSOR12_MASK, SENSOR12_ATTR, sensor_irq_handler12);
	NVIC_EnableIRQ((IRQn_Type) SENSOR12_ID);
	pio_handler_set_priority(SENSOR12_PIO,
			(IRQn_Type) SENSOR12_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR12_PIO, SENSOR12_MASK);

}

#define SENSOR13_ID ID_PIOC
#define SENSOR13_PIO PIOC
#define SENSOR13_MASK  PIO_PC23
#define IRQ_PRIOR_PIO 0
#define SENSOR13_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler13(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler13(void)
{
	pmc_enable_periph_clk(SENSOR13_ID);
	pio_set_debounce_filter(SENSOR13_PIO, SENSOR13_MASK, 10);
	
	pio_handler_set(SENSOR13_PIO, SENSOR13_ID,
			SENSOR13_MASK, SENSOR13_ATTR, sensor_irq_handler13);
	NVIC_EnableIRQ((IRQn_Type) SENSOR13_ID);
	pio_handler_set_priority(SENSOR13_PIO,
			(IRQn_Type) SENSOR13_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR13_PIO, SENSOR13_MASK);

}


#define SENSOR14_ID ID_PIOC
#define SENSOR14_PIO PIOC
#define SENSOR14_MASK  PIO_PC16
#define IRQ_PRIOR_PIO 0
#define SENSOR14_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler14(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler14(void)
{
	pmc_enable_periph_clk(SENSOR14_ID);
	pio_set_debounce_filter(SENSOR14_PIO, SENSOR14_MASK, 10);
	
	pio_handler_set(SENSOR14_PIO, SENSOR14_ID,
			SENSOR14_MASK, SENSOR14_ATTR, sensor_irq_handler14);
	NVIC_EnableIRQ((IRQn_Type) SENSOR14_ID);
	pio_handler_set_priority(SENSOR14_PIO,
			(IRQn_Type) SENSOR14_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR14_PIO, SENSOR14_MASK);

}

#define SENSOR15_ID ID_PIOC
#define SENSOR15_PIO PIOC
#define SENSOR15_MASK  PIO_PC11
#define IRQ_PRIOR_PIO 0
#define SENSOR15_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler15(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler15(void)
{
	pmc_enable_periph_clk(SENSOR15_ID);
	pio_set_debounce_filter(SENSOR15_PIO, SENSOR15_MASK, 10);
	
	pio_handler_set(SENSOR15_PIO, SENSOR15_ID,
			SENSOR15_MASK, SENSOR15_ATTR, sensor_irq_handler15);
	NVIC_EnableIRQ((IRQn_Type) SENSOR15_ID);
	pio_handler_set_priority(SENSOR15_PIO,
			(IRQn_Type) SENSOR15_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR15_PIO, SENSOR15_MASK);

}

#define SENSOR16_ID ID_PIOC
#define SENSOR16_PIO PIOC
#define SENSOR16_MASK  PIO_PC9
#define IRQ_PRIOR_PIO 0
#define SENSOR16_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler16(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler16(void)
{
	pmc_enable_periph_clk(SENSOR16_ID);
	pio_set_debounce_filter(SENSOR16_PIO, SENSOR16_MASK, 10);
	
	pio_handler_set(SENSOR16_PIO, SENSOR16_ID,
			SENSOR16_MASK, SENSOR16_ATTR, sensor_irq_handler16);
	NVIC_EnableIRQ((IRQn_Type) SENSOR16_ID);
	pio_handler_set_priority(SENSOR16_PIO,
			(IRQn_Type) SENSOR16_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR16_PIO, SENSOR16_MASK);

}

#define SENSOR17_ID ID_PIOC
#define SENSOR17_PIO PIOC
#define SENSOR17_MASK  PIO_PC6
#define IRQ_PRIOR_PIO 0
#define SENSOR17_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler17(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler17(void)
{
	pmc_enable_periph_clk(SENSOR17_ID);
	pio_set_debounce_filter(SENSOR17_PIO, SENSOR17_MASK, 10);
	
	pio_handler_set(SENSOR17_PIO, SENSOR17_ID,
			SENSOR17_MASK, SENSOR17_ATTR, sensor_irq_handler17);
	NVIC_EnableIRQ((IRQn_Type) SENSOR17_ID);
	pio_handler_set_priority(SENSOR17_PIO,
			(IRQn_Type) SENSOR17_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR17_PIO, SENSOR17_MASK);

}

#define SENSOR18_ID ID_PIOC
#define SENSOR18_PIO PIOC
#define SENSOR18_MASK  PIO_PC7
#define IRQ_PRIOR_PIO 0
#define SENSOR18_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler18(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler18(void)
{
	pmc_enable_periph_clk(SENSOR18_ID);
	pio_set_debounce_filter(SENSOR18_PIO, SENSOR18_MASK, 10);
	
	pio_handler_set(SENSOR18_PIO, SENSOR18_ID,
			SENSOR18_MASK, SENSOR18_ATTR, sensor_irq_handler18);
	NVIC_EnableIRQ((IRQn_Type) SENSOR18_ID);
	pio_handler_set_priority(SENSOR18_PIO,
			(IRQn_Type) SENSOR18_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR18_PIO, SENSOR18_MASK);

}

#define SENSOR19_ID ID_PIOC
#define SENSOR19_PIO PIOC
#define SENSOR19_MASK  PIO_PC3
#define IRQ_PRIOR_PIO 0
#define SENSOR19_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler19(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler19(void)
{
	pmc_enable_periph_clk(SENSOR19_ID);
	pio_set_debounce_filter(SENSOR19_PIO, SENSOR19_MASK, 10);
	
	pio_handler_set(SENSOR19_PIO, SENSOR19_ID,
			SENSOR19_MASK, SENSOR19_ATTR, sensor_irq_handler19);
	NVIC_EnableIRQ((IRQn_Type) SENSOR19_ID);
	pio_handler_set_priority(SENSOR19_PIO,
			(IRQn_Type) SENSOR19_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR19_PIO, SENSOR19_MASK);

}
 

#define SENSOR20_ID ID_PIOC
#define SENSOR20_PIO PIOC
#define SENSOR20_MASK  PIO_PC1
#define IRQ_PRIOR_PIO 0
#define SENSOR20_ATTR PIO_IT_RISE_EDGE

static void sensor_irq_handler20(uint32_t a , uint32_t b)
{
	printf("%s\n",__func__);
}

 void config_sensor_irq_handler20(void)
{

	pmc_enable_periph_clk(SENSOR20_ID);
	pio_set_debounce_filter(SENSOR20_PIO, SENSOR20_MASK, 10);
	
	pio_handler_set(SENSOR20_PIO, SENSOR20_ID,
			SENSOR20_MASK, SENSOR20_ATTR, sensor_irq_handler20);
	NVIC_EnableIRQ((IRQn_Type) SENSOR20_ID);
	pio_handler_set_priority(SENSOR20_PIO,
			(IRQn_Type) SENSOR20_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(SENSOR20_PIO, SENSOR20_MASK);

}



/*********************************sensor interrpt mode end*************************************/