/*
 * system.c
 *
 * Created: 11.10.2014 18:40:47
 *  Author: Tomas Baca
 */

#include "sysclk.h"
#include "system.h"
#include "config.h"
#include "TC_driver.h"
#include "ioport.h"
#include "leds.h"

UsartBuffer * pc_usart_buffer;

TWI_Master_t twi_csp_master;		/*!< TWI slave module. */
TWI_Slave_t twi_csp_slave;			/*!< TWI slave module. */
 
/* -------------------------------------------------------------------- */
/*	Initialize the xMega peripherals									*/
/* -------------------------------------------------------------------- */
void boardInit() {
	
	// prepare the i/o for LEDs
	ioport_init();
	
	// clock init & enable system clock to all peripheral modules
	sysclk_init();
	sysclk_enable_module(SYSCLK_PORT_GEN, 0xff);
	sysclk_enable_module(SYSCLK_PORT_A, 0xff);
	sysclk_enable_module(SYSCLK_PORT_B, 0xff);
	sysclk_enable_module(SYSCLK_PORT_C, 0xff);
	sysclk_enable_module(SYSCLK_PORT_D, 0xff);
	sysclk_enable_module(SYSCLK_PORT_E, 0xff);
	sysclk_enable_module(SYSCLK_PORT_F, 0xff);
	
	ioport_set_pin_dir(RED, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(YELLOW, IOPORT_DIR_OUTPUT);
	
	led_yellow_off();
	led_red_off();
		
	pc_usart_buffer = usartBufferInitialize(&PC_USART, PC_USART_BAUDRATE, PC_USART_BUFFERSIZE);
	
	/* -------------------------------------------------------------------- */
	/*	i2C init															*/
	/* -------------------------------------------------------------------- */
	
	// Initialize TWI master for CSP
	TWI_MasterInit(&twi_csp_master, &CSP_I2C_INTERFACE, TWI_MASTER_INTLVL_LO_gc, CSP_I2C_BAUDSETTING);
		
	// Initialize TWI slave for CSP
	TWI_SlaveInitializeDriver(&twi_csp_slave, &CSP_I2C_INTERFACE, TWI_CSP_SlaveProcessData);
	TWI_SlaveInitializeModule(&twi_csp_slave, CSP_MY_ADDRESS, TWI_SLAVE_INTLVL_LO_gc);
}

// this function processes received data on the I2C Slave line
// It is call by the I2C driver
void TWI_CSP_SlaveProcessData(void) {
	
	uint8_t ch;
	
	if (twi_csp_slave.result == TWIS_RESULT_OK) {
		
		if (twi_csp_slave.bytesReceived > 0) {
			
			usartBufferPutByte(pc_usart_buffer, '0', 100);
			usartBufferPutByte(pc_usart_buffer, '|', 100);
			
			// copy the data
			uint16_t i;
			for (i = 0; i < twi_csp_slave.bytesReceived; i++) {
				
				// creates the first hex character
				ch = twi_csp_slave.receivedData[i];
				ch = ch & 240;
				ch = ch >> 4;
				if (ch >= 0 && ch <= 9)
					ch = ch + '0';
				else
					ch = ch + 'A' - 10;
				usartBufferPutByte(pc_usart_buffer, ch, 100);
				
				// creates the second hex character			
				ch = twi_csp_slave.receivedData[i];
				ch = ch & 15;
				if (ch >= 0 && ch <= 9)
					ch = ch + '0';
				else
					ch = ch + 'A' - 10;
				usartBufferPutByte(pc_usart_buffer, ch, 100);
								
				// newFrame.data[i] = twi_csp_slave.receivedData[i];
			}
				
			usartBufferPutByte(pc_usart_buffer, '\n', 100);
			ledRed = 1;
		}
	}
}

/**
 * Send I2C frame via the selected device
 *
 * @param handle Handle to the device
 * @param frame Pointer to I2C frame
 * @param timeout Ticks to wait
 * @return Error code
 */
int i2c_send(int8_t destination, uint8_t * buffer, int8_t data_len) {
		
	if (TWI_MasterWrite(&twi_csp_master, destination, (uint8_t *) buffer, data_len)) {
		
		return 0;
	} else {
		
		return 1;
	}
}

/*! CSP Master Interrupt vector. */
ISR(CSP_I2C_TWIM) {
	TWI_MasterInterruptHandler(&twi_csp_master);
}

/*! CPS Slave Interrupt vector. */
ISR(CSP_I2C_TWIS) {
	TWI_SlaveInterruptHandler(&twi_csp_slave);
}