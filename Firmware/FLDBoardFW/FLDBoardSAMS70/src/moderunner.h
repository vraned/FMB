/*
 *  moderunner.h
 *
 *  Created: 12/16/2015 
 *  Author: David Florez
 */ 


#ifndef MODERUNNER_H_
#define MODERUNNER_H_


#define SENSOR20_ID ID_PIOC
#define SENSOR20_PIO PIOC
#define SENSOR20_MASK  PIO_PC1
#define IRQ_PRIOR_PIO 0
#define SENSOR20_ATTR PIO_IT_RISE_EDGE

void vModeRunnerTask( void *pvParameters );




#endif /* MODERUNNER_H_ */