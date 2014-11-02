/*
 * csp_init.c
 *
 * Created: 29.9.2014 21:39:45
 *  Author: Tomas Baca
 */

#include "i2c.h"
#include "twi_master_driver.h"
#include "twi_slave_driver.h"
#include "ioport.h"
#include "system.h"

TWI_Master_t twi_csp_master;		/*!< TWI slave module. */
TWI_Slave_t twi_csp_slave;			/*!< TWI slave module. */
i2c_callback_t callbackFunction;

// this function processes received data on the I2C Slave line
// It is call by the I2C driver
void TWI_CSP_SlaveProcessData(void) {
	
	if (twi_csp_slave.result == TWIS_RESULT_OK) {
		
		i2c_frame_t newFrame;
		
		if (twi_csp_slave.bytesReceived > 0) {
			
			// set the data len
			newFrame.len = twi_csp_slave.bytesReceived;
			
			// copy the data
			int i;
			for (i = 0; i < twi_csp_slave.bytesReceived; i++) {
				
				newFrame.data[i] = twi_csp_slave.receivedData[i];
			}
		}
		
		callbackFunction(&newFrame, false);
		
	}
}

// Initialize the i2c interface for CSP
int i2c_init(int handle, int mode, uint8_t addr, uint16_t speed, int queue_len_tx, int queue_len_rx, i2c_callback_t callback) {
	
	// Initialize TWI master for CSP
	TWI_MasterInit(&twi_csp_master, &CSP_I2C_INTERFACE, TWI_MASTER_INTLVL_LO_gc, speed);
			
	// Initialize TWI slave for CSP
	TWI_SlaveInitializeDriver(&twi_csp_slave, &CSP_I2C_INTERFACE, TWI_CSP_SlaveProcessData);
	TWI_SlaveInitializeModule(&twi_csp_slave, addr, TWI_SLAVE_INTLVL_LO_gc);
	
	callbackFunction = callback;
	
	return E_NO_ERR;
}

/**
 * Send I2C frame via the selected device
 *
 * @param handle Handle to the device
 * @param frame Pointer to I2C frame
 * @param timeout Ticks to wait
 * @return Error code
 */
int i2c_send(int handle, i2c_frame_t * frame, uint16_t timeout) {
		
	if (TWI_MasterWrite(&twi_csp_master, frame->dest, (uint8_t *) frame->data, frame->len)) {
		
		return E_NO_ERR;
	} else {
		
		return 1;
	}
}

/* User functions required */
void clock_get_time(csp_timestamp_t * time) {
	
	
}

extern void clock_set_time(csp_timestamp_t * time) {
	
	
}

/*! CSP Master Interrupt vector. */
ISR(CSP_I2C_TWIM)
{
	TWI_MasterInterruptHandler(&twi_csp_master);
}

/*! CPS Slave Interrupt vector. */
ISR(CSP_I2C_TWIS)
{
	TWI_SlaveInterruptHandler(&twi_csp_slave);
}