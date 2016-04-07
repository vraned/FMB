/*
 * comms.h
 *
 *  Created: 12/2/2015 
 *  Author: David Florez
 */ 

#ifndef COMMS_H_
#define COMMS_H_

// Task Prototypes
void vMSGTask( void *pvParameters );

// USB Callback
void usb_rx_notify(uint8_t port);



#endif /* COMMS_H_ */