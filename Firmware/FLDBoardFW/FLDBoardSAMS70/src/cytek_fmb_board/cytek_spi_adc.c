/*
 * cytek_spi.c
 *
 * Created: 2015/11/17/周二 15:10:39
 *  Author: lenovo
 */ 
 #include <cytek_config.h>
#include <ioport.h>
#include <cytek_time.h>
#include <cytek_ioport.h>
#include <cytek_spi_adc.h>
#include <spi.h>
#include "mode_table.h"

extern mode_table_t ModeTable;



//void spi_master_initialize(Spi *p_spi,uint8_t ul_pcs_ch,uint32_t set_bits);
 //void spi_read_ch(Spi *p_spi,uint8_t *ul_pcs,uint32_t set_bits,uint16_t *data);
 //uint16_t spi_read_gpio( uint16_t gpio_cs,uint16_t gpio_clk,uint16_t data_miso, uint16_t data_mosi,uint8_t read_bits);


/**
	*initialize the spi port.
	*p_spi: point to 
	* ul_pcs: npcs
	* set_bits: read bits , just like SPI_CSR_BITS_11_BIT,and so on
**/
 void spi_master_initialize(Spi *p_spi,uint8_t ul_pcs_ch,uint32_t set_bits)
 {
	 //puts("-I- Initialize SPI as master\r");
 
	 /* Configure an SPI peripheral. */
	 spi_enable_clock(p_spi);
	 spi_disable(p_spi);
	 spi_reset(p_spi);
	 spi_set_lastxfer(p_spi);
	 spi_set_master_mode(p_spi);
	 spi_disable_mode_fault_detect(p_spi);
	 spi_set_peripheral_chip_select_value(p_spi, spi_get_pcs(ul_pcs_ch));
	 spi_set_clock_polarity(p_spi, ul_pcs_ch, 0); 
	 spi_set_clock_phase(p_spi, ul_pcs_ch, 1);
	 spi_set_bits_per_transfer(p_spi, ul_pcs_ch,set_bits);
	 spi_set_baudrate_div(p_spi, ul_pcs_ch,(sysclk_get_peripheral_hz()/500000));
	 spi_set_transfer_delay(p_spi, ul_pcs_ch, 0x80,0xff);
	 //spi_set_transfer_delay(p_spi, ul_pcs_ch, 0x80,0x180);
	 spi_enable(p_spi);
 }

/*** spi_read_ch
	*p_spi:point to the spi to select
	* ul_pcs: npcs,store the last pcs selected to transmit if the spi is in master mod
	* data: point to read buf
	* set_bits: read bits , just like SPI_CSR_BITS_11_BIT,and so on
***/

void spi_read_ch(Spi *p_spi,uint8_t *ul_pcs,uint32_t set_bits,uint16_t *data)
{
	spi_status_t err;
	uint8_t temp_pcs = *ul_pcs;
	spi_master_initialize(p_spi,*ul_pcs,set_bits);
	/*must have a write to receive the read data,according to the spec*/
	err = spi_write(p_spi,0xff,*ul_pcs,1);
	
	if(err)
		printf("SPI%d write ERROR while read pcs%d \n",(p_spi==SPI0?0:1),*ul_pcs);
	
	err = spi_read(p_spi,data,ul_pcs);

	if(err)
		printf("SPI%d read pcs%d  ERROR\n",(p_spi==SPI0?0:1),*ul_pcs);

	if(temp_pcs != *ul_pcs)
		printf("SPI%d: the pcs read and write is NOT the same one!\n ",(p_spi==SPI0?0:1));
	return;
}


uint16_t read_pressure_vacuum_origin(void)
{
	uint8_t npcs = 0;
	uint16_t temp;
	spi_read_ch(SPI1,&npcs,SPI_CSR_BITS_16_BIT,&temp);
	printf("%s %u\n",__func__,temp);
	return temp;
}

uint16_t read_pressure_sample_origin(void)
{
	uint8_t npcs = 1;
	uint16_t temp;
	spi_read_ch(SPI1,&npcs,SPI_CSR_BITS_16_BIT,&temp);
	printf("%s 0x%x\n",__func__,temp);
	return temp;
}

uint16_t  read_temperature_origin(void)
{
	uint8_t npcs = 2;
	uint16_t temp = 0;
	spi_read_ch(SPI1,&npcs,SPI_CSR_BITS_11_BIT,&temp);
	printf("%s 0x%x\n",__func__,temp);
	return temp;
}

float get_thermistor_temp(void)
{
	uint16_t tmp = read_temperature_origin();
    uint16_t tmp2 = tmp & 0x1FF;
    float temp =  tmp2 * .125F;
    
    return temp;
}

float get_sample_xducer(void)
{
	float press;

	uint16_t sample = read_pressure_sample_origin();
	if (sample > ModeTable.calibration_values.sample_offset)
	{
		press = (float)(sample - ModeTable.calibration_values.sample_offset) / (ModeTable.calibration_values.sample_gain);
	}
	else
	{
		press = 0;
	}
    
	return press;
}

float get_diff_xducer(void)
{
	float press;

	uint16_t diff = read_pressure_vacuum_origin(); 
	if (diff > ModeTable.calibration_values.diff_offset)
	{
		press = (float)(diff - ModeTable.calibration_values.diff_offset) / (ModeTable.calibration_values.diff_gain);
    }
    else
    {
	    press = 0;
    }
    
    return press;
}

/*
float  read_temperature_float(void)
{
	uint8_t npcs = 2;
	uint16_t temp_origin = 0;
	float temp = 0;
	spi_read_ch(SPI1,&npcs,SPI_CSR_BITS_11_BIT,&temp_origin);
	temp = (temp_origin >> 3 )+ ( ( temp_origin& 0x07) * 0.125);
	return temp;
}
*/

/**example : SPI1 read pcs0,pcs1,pcs2
	while(1){
				ch=0;
				spi_read_ch(SPI1,&ch,SPI_CSR_BITS_16_BIT,&spi_data);
				printf("SPI1 read from pcs	0x%x: %d\n",ch,spi_data);
				if(state) //ensure that the program running
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_LOW);
				else
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_HIGH);
				state = !state;
				mdelay(100);
				
				ch = 1;
				spi_read_ch(SPI1,&ch,SPI_CSR_BITS_16_BIT,&spi_data);
				printf("SPI1 read from pcs	0x%x: %d\n",ch,spi_data);
				if(state) //ensure that the program running
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_LOW);
				else
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_HIGH);
				state = !state;
				mdelay(100);
				
				ch = 2;
				spi_read_ch(SPI1,&ch,SPI_CSR_BITS_11_BIT,&spi_data);
				printf("SPI1 read TEMP	from pcs  0x%x: %d\n",ch,spi_data);
				if(state) //ensure that the program running
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_LOW);
				else
					ioport_set_pin_level(PIO_PA20_IDX,IOPORT_PIN_LEVEL_HIGH);
				state = !state;
				mdelay(100);

				}
**/

