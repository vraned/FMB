/*
 *  mode_table.h
 *
 *  Created: 12/2/2015
 *  Author: David Florez
 */ 


#ifndef MODETABLE_H_
#define MODETABLE_H_

#define MAX_NUMBER_OF_MODES 20
#define MAX_NUMBER_OF_TIMESEGMENTS 15

#define NUMBER_KT_ENTRIES          20
#define NUMBER_PRESSURE_ENTRIES     1
#define NUMBER_SAMPLE_FLOW_ENTRIES  3
#define NUMBER_SIT_ENTRIES          3
#define NUMBER_OF_VALVES            6 
#define NUMBER_OF_PUMPS             4
#define NUMBER_OF_LASERS            3

typedef enum {PR_DIFF = 0} pressure_t;
typedef enum {SF_LOW = 0, SF_MED, SF_HIGH} sample_flow_t;
typedef enum {POS_SITU = 0, POS_SITD, POS_SITP} pos_sit_t;
typedef enum {T_SITU = NUMBER_KT_ENTRIES + 1, T_SITD, T_SITP, T_TUBE, T_NO_TUBE, T_TOGG} m_time_t;
typedef enum {VACCUM = 0, P1, P2, P3} m_pumps_t;
typedef enum {MS_LOW = 0, MS_MED, MS_HIGH, MS_DYN, MS_HOLD} m_sample_flowrate_t;
    
    
typedef struct   
{
    m_time_t time;
    uint8_t valves[NUMBER_OF_VALVES];
    uint8_t pumps[NUMBER_OF_PUMPS];
    m_sample_flowrate_t sample;
} time_segment_t;
    
typedef struct 
{
    float pressure[NUMBER_PRESSURE_ENTRIES];
    uint32_t sample_flow[NUMBER_SAMPLE_FLOW_ENTRIES];
    uint32_t sit[NUMBER_SIT_ENTRIES];
    
} calibration_table_t;

typedef struct  
{   
    unsigned   diff_gain;
    unsigned   diff_ref;
    unsigned   sample_gain;
    unsigned   sample_ref;
    unsigned   diff_offset;
    unsigned   sample_offset; 
} calibration_values_t;

typedef struct 
{
    uint32_t nModes;
	time_segment_t mode[MAX_NUMBER_OF_MODES][MAX_NUMBER_OF_TIMESEGMENTS];
    calibration_table_t calibration_table;
    calibration_values_t calibration_values;
    float laser_delay[NUMBER_OF_LASERS];
    float temp_factor, ref_temp, press_tol;
    float timing_table[NUMBER_KT_ENTRIES];
} mode_table_t;



#endif /* MODETABLE_H_ */