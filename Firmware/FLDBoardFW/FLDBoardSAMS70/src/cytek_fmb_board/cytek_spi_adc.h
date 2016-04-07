/*
 * cytek_spi_adc.h
 *
 * Created: 2015/11/17/周二 15:10:56
 *  Author: lenovo
 */ 


#ifndef CYTEK_SPI_ADC_H_
#define CYTEK_SPI_ADC_H_

void spi_master_initialize(Spi *p_spi,uint8_t ul_pcs_ch,uint32_t set_bits);
void spi_read_ch(Spi *p_spi,uint8_t *ul_pcs,uint32_t set_bits,uint16_t *data);
uint16_t read_pressure_vacuum_origin(void);
uint16_t read_pressure_sample_origin(void);
uint16_t  read_temperature_origin(void);
//float  read_temperature_float(void);
float   get_thermistor_temp(void);
float   get_sample_xducer(void);
float   get_diff_xducer(void);

#endif /* CYTEK_SPI_ADC_H_ */