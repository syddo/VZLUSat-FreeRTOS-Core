/*
 * config.h
 *
 * Created: 11.10.2014 18:31:37
 *  Author: Tomas Baca
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include "twi_master_driver.h"

// Address of the local CSP node
#define CSP_MY_ADDRESS	1

// Actual position of CSP i2c on the xMega
// Replace "TWIE" with adequate port of your board
#define CSP_I2C_INTERFACE	TWIC
#define CSP_I2C_TWIM		TWIC_TWIM_vect
#define CSP_I2C_TWIS		TWIC_TWIS_vect

// Baudrate of the CSP i2c interface
#define CSP_I2C_BAUDRATE	400000
#define CSP_I2C_BAUDSETTING TWI_BAUD(F_CPU, CSP_I2C_BAUDRATE)

// define the usart port for MEDIPIX
#define PC_USART	USARTD0

#define PC_USART_BAUDRATE	BAUD115200

#define PC_USART_BUFFERSIZE	255

#endif /* CONFIG_H_ */