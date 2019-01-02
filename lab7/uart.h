#ifndef _UART_H
#define _UART_H

#define BIT(n)			(0x01<<(n))

#define COM1_IRQ			4
#define COM1_BASE_ADDRESS	0x3F8
#define COM1_VECTOR			0x0C

#define COM2_IRQ			3
#define COM2_BASE_ADDRESS	0x2F8
#define COM2_VECTOR			0x0B

#define UART_RBR			0		//Receiver Buffer Register			R
#define UART_THR			0		//Transmitter Holding Register		W
#define UART_IER			1		//Interrupt Enable Register			R/W
#define UART_IIR			2		//Interrupt Identification Reg.		R
#define UART_FCR			2		//FIFO Control Register				W
#define UART_LCR			3		//Line Control Register				R/W
#define UART_MCR			4		//Modem Control Register			R/W
#define UART_LSR			5		//Line Status Register				R
#define UART_MSR			6		//Modem Status Register				R
#define UART_SR				7		//Scratchpad Register				R/W

//If DLAB register of the LCR (Line Control Register) is set to 1
#define UART_DLL			0		//Divisor Latch LSB					R/W
#define UART_DLM			1		//Divisor Latch MSB					R/W

//Line Control Register (LCR)
#define FIVE_BPC			0		//5 bits per char
#define SIX_BPC				1		//6 bits per char
#define SEVEN_BPC			2		//7 bits per char
#define EIGHT_BPC			3		//8 bits per char
#define STOP_BITS			BIT(2)	//0 for 1 stop bit, 1 for 2 stop bits (1,5 in 5 bits per char)
#define par_control			BIT(3)	//if 0, no parity
#define	odd_par				8		//Odd parity
#define even_par			24		//Even parity
#define par_bit_1			40		//parity bit is always 1
#define par_bit_0			56		//parity bit is always 0
#define	break_control		BIT(6)	//sets serial output to 0 (low)
#define DLAB				BIT(7)	//Divisor Latch Access

//Line Status Register (LSR)
#define SER_REC_DATA		BIT(0)	//Set to 1 when there is data for receiving
#define SER_OVERRUN_ERR		BIT(1)	//Set to 1 when a characters received is overwritten by another one
#define SER_PARITY_ERR		BIT(2)	//Set to 1 when a character with a parity error is received
#define	SER_FRAME_ERR		BIT(3)	//Set to 1 when a received character does not have a valid Stop bit
#define	SER_BREAK_INT		BIT(4)	//Set to 1 when the serial data input line is held in the low level for longer than a full “word” transmission
#define SER_THR_EMPTY		BIT(5)	//When set, means that the UART is ready to accept a new character for transmitting
#define	SER_TRAN_EMPTY		BIT(6)	//When set, means that both the THR and the Transmitter Shift Register are both empty
#define	SER_FIFO_ERR		BIT(7)	//Set to 1 when there is at least one parity error or fram-
									//ing error or break indication in the FIFO
									//Reset to 0 when the LSR is read, if there are no sub-
									//sequent errors in the FIFO
									
typedef enum base_address{COM1=0x3F8, COM2=0x2F8} base_address;

typedef enum parity_control{none=-1, even=0, odd=1} parity_control;

uint8_t uart_get_conf(base_address base_addr);

uint8_t uart_set(base_address base_addr, uint8_t bits, uint8_t stop, parity_control parity, unsigned long rate);

uint8_t uart_get_polled(base_address base_addr, uint32_t *character);

uint8_t uart_get_polled_word(base_address base_addr, char **word);

uint8_t uart_send_polled(base_address base_addr, uint32_t character);

uint8_t uart_send_polled_word(base_address base_addr, char *word);

#endif
