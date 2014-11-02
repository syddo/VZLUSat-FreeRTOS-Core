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
// #define CSP_MY_ADDRESS	2	// Deska EPS
// #define CSP_MY_ADDRESS	3	// Deska Measure
#define CSP_MY_ADDRESS	4		// Deska Medipix

// Address of the OBC CSP node
#define CSP_OBC_ADDRESS	1	

// Actual position of CSP i2c on the xMega
// Replace "TWIE" with adequate port of your board
#define CSP_I2C_INTERFACE	TWIE
#define CSP_I2C_TWIM		TWIE_TWIM_vect
#define CSP_I2C_TWIS		TWIE_TWIS_vect

// Slave address of my CSP i2c interface
#define CSP_I2C_SLAVE_ADDRESS	CSP_MY_ADDRESS

// Slave address of the OBC CSP i2c interface
#define CSP_I2C_OBC_ADDRESS	CSP_OBC_ADDRESS

// Number of CSP buffer to allocate
#define CSP_BUFFER_COUNT	1

// CSP Buffer size in bytes
#define CSP_BUFFER_SIZE	86

// Maximum size of CSP packet, payload + header
#define CSP_PACKET_SIZE	72

// Size of CSP router stack in words
#define CSP_ROUTER_STACK	500

// Do not ever change!!!
#define CSP_ROUTER_PRIORITY	0

// Baudrate of the CSP i2c interface
#define CSP_I2C_BAUDRATE	400000
#define CSP_I2C_BAUDSETTING TWI_BAUD(F_CPU, CSP_I2C_BAUDRATE)

#endif /* CONFIG_H_ */