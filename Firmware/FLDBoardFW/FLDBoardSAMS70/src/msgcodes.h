/*
 *  msgcodes.h
 *
 *  Created: 12/3/2015 
 *  Author: David Florez
 */ 


#ifndef MSGCODES_H_
#define MSGCODES_H_

#define MSG_LOAD_MODE_TABLE     0x11

#define MSG_DUMMY_MESSAGE       0x12
#define MSG_E_STOP              0x13
#define MSG_SET_PRESS           0x14
#define MSG_GET_PRESS           0x15
#define MSG_SET_PRESS_TOL       0x16
#define MSG_GET_PRESS_TOL       0x17
#define MSG_SET_REF_TEMP        0x18
#define MSG_GET_REF_TEMP        0x34
#define MSG_GET_TEMP            0x19
#define MSG_SET_TEMP_FACTOR     0x1A
#define MSG_GET_TEMP_FACTOR     0x1B
#define MSG_EXEC_MODE           0x1C
#define MSG_SET_CAL_VALUES      0x1D
#define MSG_GET_CAL_VALUES      0x26
#define MSG_GET_SENSOR          0x1E
#define MSG_GET_VALVE           0x1F
#define MSG_GET_PUMP            0x20
#define MSG_SET_VALVE           0x21
#define MSG_SET_PUMP            0x22
#define MSG_SET_LASER           0x23
#define MSG_GET_LASER           0x24
#define MSG_SET_LASER_DELAY     0x25
#define MSG_INCREASE_FLOW_RATE  0x29
#define MSG_DECREASE_FLOW_RATE  0x27
#define MSG_GET_SAMPLE          0x28
#define MSG_SET_FLOW_RATE       0x30
#define MSG_READ_SENSIRION		0x33

#define MSG_GET_SIT_POS 0x40
#define MSG_SET_SIT_TOL 0x41
#define MSG_GET_SIT_TOL 0x42
#define MSG_SIT_CAL 0x43
#define MSG_SIT_MOVE 0x44













#endif /* MSGCODES_H_ */
