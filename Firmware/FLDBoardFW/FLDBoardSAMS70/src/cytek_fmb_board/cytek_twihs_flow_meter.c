/*
 * cytek_twihs_flow_meter.c
 *
 * Created: 2015/12/11/周五 10:54:24
 *  Author: lenovo
 *
 */ 
 

#include <asf.h>
#include <stdio.h>
#include <twihs.h>
#include <sysclk.h>
#include <pmc.h>
#include "cytek_twihs_flow_meter.h"

#define DEBUG_FLOW_METER

#ifdef DEBUG_FLOW_METER
	char debugMsg[100];
	#define dbg_printf sprintf
#else
	#define dbg_printf(...) 
#endif


#define FLOW_METER_ADDR 	0x40
#define FLOW_METER_PORT 	TWIHS0
#define FLOW_METER_SPEED	10000
#define FLOW_METER_PORT_ID   ID_TWIHS0

#define WRITE_USER_REG 	0xE2
#define READ_USER_REG 	0xE3
#define WRITE_ADVANCE_USER_REG 	0xE4
#define READ_ADVANCE_USER_REG	0xE5
#define READ_ONLY_REG1 	0xE7
#define READ_ONLY_REG2	0xE9
#define TRIG_TEMP_MEASURE	0xF3
#define TRIG_FLOW_MEASURE	0xF1
#define TRIG_VDD_MEASURE	0xF5
#define SOFT_RESET	0xFE
#define SET_EEPROM_ADDR 		0xFA


const char *dimention[16]={
	"t.b.d","t.b.d","t.b.d",
	"n","u","m","c","d","1","-","h","k","m","G",
	"t.b.d","t.b.d",
	};
const char *time_base[16] = {
	"totalized","us","ms","s","min","h","day",
	"t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d",
	};
const char *volume[32]={
	"norm liter",
	"standard liter",
	"t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d",
	"liter",
	"gram",
	"t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d",
	"pascal",
	"bar",
	"meter H2O",
	"inch H2O",
	"t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d","t.b.d",
	};


/**the function is as the same as twihs_mk_addr()
 * \internal
 * \brief Construct the TWIHS module address register field
 *
 * The TWIHS module address register is sent out MSB first. And the size controls
 * which byte is the MSB to start with.
 *
 * Please see the device datasheet for details on this.
 */
static uint32_t twihs_mk_addr_flow(const uint8_t *addr, int len)
{
	uint32_t val;

	if (len == 0)
		return 0;

	val = addr[0];
	if (len > 1) {
		val <<= 8;
		val |= addr[1];
	}
	if (len > 2) {
		val <<= 8;
		val |= addr[2];
	}
	return val;
}



static uint32_t flow_meter_write(Twihs *p_twihs, twihs_packet_t *p_packet)
{
		uint32_t status, cnt = p_packet->length;
		uint8_t *buffer = p_packet->buffer;

#if 0
		/* Check argument */

		if (cnt == 0) {
			return TWIHS_INVALID_ARGUMENT;
		}
#endif
		/* Set write mode, slave address and 3 internal address byte lengths */
		p_twihs->TWIHS_MMR = 0;
		p_twihs->TWIHS_MMR = TWIHS_MMR_DADR(p_packet->chip) |
				((p_packet->addr_length << TWIHS_MMR_IADRSZ_Pos) &
				TWIHS_MMR_IADRSZ_Msk);
	
		/* Set internal address for remote chip */
		p_twihs->TWIHS_IADR = 0;
		p_twihs->TWIHS_IADR = twihs_mk_addr_flow(p_packet->addr, p_packet->addr_length);
	
		/* Send all bytes */
		while (cnt > 0) {
			status = p_twihs->TWIHS_SR;
			if (status & TWIHS_SR_NACK) {
				return TWIHS_RECEIVE_NACK;
			}
	
			if (!(status & TWIHS_SR_TXRDY)) {
				continue;
			}
			p_twihs->TWIHS_THR = *buffer++;
	
			cnt--;
			dbg_printf(debugMsg, "1111\r");
		}

		while (1) {
			delay_us(4000);
			status = p_twihs->TWIHS_SR;    //read the SR after the last byte be transmit
			dbg_printf(debugMsg, "SR  0x%x\n",status);
			if (status & TWIHS_SR_NACK) {
				return TWIHS_RECEIVE_NACK;
			}
	
			if (status & TWIHS_SR_TXRDY) {
				break;
			}
			dbg_printf(debugMsg, "2222\r");
		}

		p_twihs->TWIHS_CR = TWIHS_CR_STOP;
	
		while (!(p_twihs->TWIHS_SR & TWIHS_SR_TXCOMP)) {
			dbg_printf(debugMsg, "333\r");
		}

		p_twihs->TWIHS_SR;   //zrw add
		return TWIHS_SUCCESS;
}


static  uint32_t flow_meter_write_register(uint8_t reg, uint16_t data )
 {
	 twihs_packet_t packet_rx; // = {0};
	 uint8_t buffer[3] = {0};
	 uint32_t result;
	 buffer[0] = reg;
	 buffer[1] = (data >> 8 ) & 0xff;
	 buffer[2] = data & 0xff;
	 packet_rx.chip = FLOW_METER_ADDR;
	 packet_rx.buffer = buffer;
	 packet_rx.length = 3;
	 result = flow_meter_write(FLOW_METER_PORT,&packet_rx);
	 if( result  == TWIHS_SUCCESS )
		 return result;
	 else{
		 dbg_printf(debugMsg, "ERROR:  %s  %d\n",__func__,result);
		 return -1;
	 }
 }

 
  /*flow_meter_write_command: send a control command to flow meter*/
static  uint32_t flow_meter_write_command(uint8_t command)
 {
	 twihs_packet_t packet_rx; // = {0};
	 uint32_t result;
	 packet_rx.chip = FLOW_METER_ADDR;
	 packet_rx.buffer = &command;
	 packet_rx.length = 1;
	 result = flow_meter_write(FLOW_METER_PORT,&packet_rx);
	 if( result  == TWIHS_SUCCESS )
		 return result;
	 else{
		 dbg_printf(debugMsg, "ERROR:  %s  %d\n",__func__,result);
		 return -1;
	 }	 
 
 }


/*twihs_master_read_byte()
	read bytes from twihs, its function is the same as twihs_master_read(),
	but the later function is not stable for flow meter, so I rewrite here and modify it
*/
static uint32_t twihs_master_read_byte(Twihs *p_twihs, twihs_packet_t *p_packet)
{
	uint32_t status, cnt = p_packet->length;
	uint8_t *buffer = p_packet->buffer;
	uint32_t timeout = TWIHS_TIMEOUT;

	/* Check argument */
	if (cnt == 0) {
		return TWIHS_INVALID_ARGUMENT;
	}

	/* Set read mode, slave address and 3 internal address byte lengths */
	p_twihs->TWIHS_MMR = 0;
	p_twihs->TWIHS_MMR = TWIHS_MMR_MREAD | TWIHS_MMR_DADR(p_packet->chip) |
			((p_packet->addr_length << TWIHS_MMR_IADRSZ_Pos) &
			TWIHS_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
	p_twihs->TWIHS_IADR = 0;
	p_twihs->TWIHS_IADR = twihs_mk_addr_flow(p_packet->addr, p_packet->addr_length);

	/* Send a START Condition */
	p_twihs->TWIHS_CR = TWIHS_CR_START;

	while (cnt > 0) {
		/*
		the flow meter stop condition is not very stable to change the twihs RXRDY bit
		in SR register during the continously read,so add a delay time, but the time to be set 
		must  NO longer than a high pulse width ,add by zrw 
		*/
		delay_us(100);
		
		status = p_twihs->TWIHS_SR;
		if (status & TWIHS_SR_NACK) {
			return TWIHS_RECEIVE_NACK;
		}
		if (!timeout--) {
			
			return TWIHS_ERROR_TIMEOUT;
		}

		/* Last byte ? */
		if (cnt == 1) {
			p_twihs->TWIHS_CR = TWIHS_CR_STOP;
		}

		if (!(status & TWIHS_SR_RXRDY)) {
			continue;
		}
		*buffer++ = p_twihs->TWIHS_RHR;
		dbg_printf(debugMsg, "buffer: 0x%x\n", *(buffer-1));
		cnt--;
		timeout = TWIHS_TIMEOUT;
	}

	while (!(p_twihs->TWIHS_SR & TWIHS_SR_TXCOMP)) {
	}
	
	/* Dummy read in status register */
	p_twihs->TWIHS_SR;

	return TWIHS_SUCCESS;
}

/*flow_meter_read
	addr max length is 3 bytes
*/
static uint32_t flow_meter_read(uint8_t *addr, uint8_t addr_len, uint8_t *buffer, uint8_t buffer_len)
{
	twihs_packet_t packet_rx;
	packet_rx.chip        = FLOW_METER_ADDR;
	packet_rx.addr_length = addr_len;
	dbg_printf(debugMsg, "%s   addr_len:%d    buffer_len:%d\n",__func__,addr_len,buffer_len);
	if(addr_len == 1 ){
		packet_rx.addr[0]	  = addr[0];
	}else if(addr_len == 2){
		packet_rx.addr[0]	  = addr[0];
		packet_rx.addr[1]	  = addr[1];
	}else{
		packet_rx.addr[0]	  = addr[0];
		packet_rx.addr[1]	  = addr[1];
		packet_rx.addr[2]	  = addr[2];
	}	
	packet_rx.buffer      = buffer;
	packet_rx.length      = buffer_len;
	return twihs_master_read_byte(FLOW_METER_PORT, &packet_rx);
}



/*    flow_meter_read_register:
	read register, register is 16 bits, and return 32 bits data
	if read failed, return -1 otherwise return the low 16 bits data
*/
static int16_t flow_meter_read_register(uint8_t reg)
{
	uint8_t buffer[2];
	int16_t result = 0;
	result = flow_meter_read(&reg,sizeof(reg),buffer,sizeof(buffer));
	if( result  == TWIHS_SUCCESS ){
		result = ( buffer[0] << 8) + buffer[1];
		dbg_printf(debugMsg, "[0]:  0x%x      [1]: 0x%x  result: 0x%x\n",buffer[0],buffer[1],result);
		return result;
	}else{
		dbg_printf(debugMsg, "ERROR:	%s  %d\n",__func__,result);
		return -1;
	}	
}

/*flow_meter_read_eeprom:
	do not use calibration filed 4 because addr is limited to 16 bit , and out of range if using cf4
*/
static uint32_t flow_meter_read_eeprom(uint16_t addr)
{
	uint8_t buffer_addr[3] = {0};
	uint8_t buffer_read[2] = {0};
	uint32_t result;
	buffer_addr[0] = SET_EEPROM_ADDR;	
	buffer_addr[1] = ( addr & 0xff00 ) >> 8;
	buffer_addr[2] = addr & 0xff;
	dbg_printf(debugMsg, " buffer_addr1= 0x%x, buffer_addr2= 0x%x \n",buffer_addr[1],buffer_addr[2]);
	result = flow_meter_read(buffer_addr,sizeof(buffer_addr),buffer_read,sizeof(buffer_read));
	if( result  == TWIHS_SUCCESS ){
		result =  (buffer_read[0] << 8) + buffer_read[1];
		dbg_printf(debugMsg, "  buffer_read0= 0x%x, buffer_read1= 0x%x  buffer_read2= 0x%x result 0x%x\n",buffer_read[0],buffer_read[1],buffer_read[2],result);
		return result;
	}else{
		dbg_printf(debugMsg, "ERROR:	%s  %d\n",__func__,result);
		return -1;
	}	
}

uint32_t flow_meter_read_user_register(void)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(READ_USER_REG);
}

uint32_t flow_meter_read_adv_user_register(void)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(READ_ADVANCE_USER_REG);
}

uint32_t flow_meter_read_only_register1(void)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(READ_ONLY_REG1);
}

uint32_t flow_meter_read_only_register2(void)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(READ_ONLY_REG2);
}

uint32_t flow_meter_write_user_register(uint16_t data)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_write_register(WRITE_USER_REG, data);
}

uint32_t flow_meter_write_adv_user_register(uint16_t data)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_write_register(WRITE_ADVANCE_USER_REG, data);
}



#if 0
/*reserved*/
void flow_meter_reset(void)
 {
	dbg_print("%s\n",__func__);
	ioport_set_pin_dir(PIO_PA27_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PA27_IDX,1);
	ioport_set_pin_level(PIO_PA27_IDX,0);
	delay_ms(100);
	ioport_set_pin_level(PIO_PA27_IDX,1);
	delay_ms(100);
 }
#endif


uint32_t flow_meter_soft_reset(void)
{
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_write_command(SOFT_RESET);
}

/*flow_meter_flow_measure:  measure the flow liquid*/
int16_t flow_meter_flow_measure(void)
 {	
 	/*the measurement action is similar with the read register*/
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(TRIG_FLOW_MEASURE);
 }

/*flow_meter_temp_measure: measure temperature */
uint32_t flow_meter_temp_measure(void)
 {	
 	/*the measurement action is similar with the read register*/
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(TRIG_TEMP_MEASURE);
 }

/*flow_meter_vdd_measure: measure vdd*/
uint32_t flow_meter_vdd_measure(void)
 {	
 	/*the measurement action is similar with the read register*/
	dbg_printf(debugMsg, "%s\n",__func__);
	return flow_meter_read_register(TRIG_VDD_MEASURE);
 }

 /*wait for floe meter vendor reply*/
uint32_t flow_meter_get_scale_factor(void)
 {
	uint32_t addr;
	int32_t result;

	dbg_printf(debugMsg, "%s\n",__func__);
	result = flow_meter_read_register(READ_USER_REG);
	if (result == -1)
		return -1;
	dbg_printf(debugMsg, " user reg: 0x%x\n",result);
	addr =( ( result & 0x70 ) >> 4 )* 0x300 + 0x2b6;
	dbg_printf(debugMsg, " addr: 0x%x\n",addr);
	addr = addr * 16;
	dbg_printf(debugMsg, " addr: 0x%x\n",addr);

	return flow_meter_read_eeprom(addr);
	
 }


uint32_t flow_meter_get_measure_unit(void)
 {
	uint32_t addr;
	int32_t result;

	dbg_printf(debugMsg, "%s\n",__func__);
	result = flow_meter_read_register(READ_USER_REG);
	if (result == -1)
		return -1;
	addr =( ( result & 0x70 ) >> 4 )* 0x300 + 0x2b7;
	addr = addr * 16;
	
	
	result =flow_meter_read_eeprom(addr);
	if (result == -1)
		return -1;
	dbg_printf(debugMsg, "Measure Unit: %s %s %s\n",dimention[result%16],volume[result/256],time_base[(result%256)/16]);
	return result;
 }


uint32_t flow_meter_init(void)
{
/*all of the deive is set as default: 
	16 bit resolution, 
	IIC addr:0x40	
*/
	twihs_options_t opt;
	uint32_t result;

	dbg_printf(debugMsg, "%s\n",__func__);
	
	/* Enable the peripheral clock for TWI */
	pmc_enable_periph_clk(ID_TWIHS0);
	delay_ms(100);  // it is important to init		

	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = FLOW_METER_SPEED;
	
	pmc_enable_periph_clk(FLOW_METER_PORT_ID);
	delay_ms(20);
	
	result = twihs_master_init(FLOW_METER_PORT, &opt);
	if ( result != TWIHS_SUCCESS) {
		dbg_printf(debugMsg, "TWI master initialization failed : %d.\r",result);
		return -1;
	}
	
	/*response time, 25 ms at least for SLS -1500 */
	delay_ms(100); 
	
	result = flow_meter_read_only_register1();
	dbg_printf(debugMsg, "init state: 0x%x\n",result);
	/*bit 10 shows wether the device init sucessfully*/
	if (result & 0x400) 
		return TWIHS_SUCCESS;
	else
		return -1;
}