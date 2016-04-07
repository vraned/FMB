/*
 * cytek_twihs_flow_meter.h
 *
 * Created: 2015/12/11/周五 10:54:58
 *  Author: lenovo
 */ 


#ifndef CYTEK_TWIHS_FLOW_METER_H_
#define CYTEK_TWIHS_FLOW_METER_H_

uint32_t flow_meter_read_user_register(void);
uint32_t flow_meter_write_user_register(uint16_t data);
uint32_t flow_meter_read_adv_user_register(void);
uint32_t flow_meter_write_adv_user_register(uint16_t data);
uint32_t flow_meter_read_only_register1(void);
uint32_t flow_meter_read_only_register2(void);

uint32_t flow_meter_init(void);
uint32_t flow_meter_soft_reset(void);

#if 0
/*reserved*/
void flow_meter_reset(void);
#endif

int16_t  flow_meter_flow_measure(void);
uint32_t flow_meter_temp_measure(void);
uint32_t flow_meter_vdd_measure(void);
uint32_t flow_meter_get_scale_factor(void);
uint32_t flow_meter_get_measure_unit(void);


#endif /* CYTEK_TWIHS_FLOW_METER_H_ */