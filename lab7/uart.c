#include <stdio.h>
#include <lcom/lcf.h>
#include <string.h>

#include "uart.h"

#define DELAY_US	2000

/*LSR and Error Detection
if( sys_inb(ser_port + UART_LSR, &st) != OK ) {
...
} else if {
if ( st & (SER_OVERRUN_ERR | SER_PARITY_ERR
| SER_FRAME_ERR) ) {
... // Handle error
}
* */


/*
 / *no error checking* /
...
/ *busy wait for transmitter ready* / 

while( !(lsr & SER_THR_EMPTY) ) {
ticksdelay();
sys_inb(ser_port + UART_LSR, &lsr);
}
/ *send character* /
sys_outb(ser_port+UART_THR, c);
* */

uint8_t uart_get_conf(base_address base_addr){
	uint32_t data = 0;

	sys_inb(base_addr + UART_LCR, &data);
	
	printf("LCR: 0x%X\n", data);

	sys_inb(base_addr + UART_IER, &data);

	printf("IER: 0x%02X\n", data);
    
    return 0;
}

uint8_t uart_set(base_address base_addr, uint8_t bits, uint8_t stop, parity_control parity, unsigned long rate){
	uint32_t lcr = 0, dll = 0, dlm = 0;
	
	if(rate <= 0){
		printf("Rate must not be zero or negative!\n");
		return -1;
	}
	
	unsigned long new_rate = 115200/rate;
	
	if(bits < 5 || bits > 8){
		printf("A char must be between 5 and 8 bits\n");
		return -1;
	}
	else if(stop < 1 || stop > 2){
		printf("You must have either 1 or 2 stop bits!\n");
		return -1;
	}
	else if(115200%new_rate){
		printf("Rate must be one of the following: 1200, 2400, 4800, 9600, 19200, 38400, 57600 or 115200\n");
		return -1;
	}

	sys_inb(base_addr + UART_LCR, &lcr);
	
	//printf("0x%02X\n", lcr);
	
	uint8_t aux = (lcr & 3);
	
	lcr -= aux;
	
	//printf("0x%02X\n", lcr);
	
	lcr = lcr | (bits-5);
	
	if(stop==2){
		lcr = lcr | STOP_BITS;
	}
	
	if(parity == odd){
		lcr = lcr | odd_par;
	}
	else if(parity == even){
		lcr = lcr | even_par;
	}
	else if(parity != none){
		printf("Parity must be either -1 for none, 0 for odd and 1 for even\n");
		return -1;
	}
	
	//printf("0x%02X\n", lcr);
	
	lcr = lcr | DLAB;
	
	//printf("0x%02X\n", lcr);
	
	sys_outb(base_addr + UART_LCR, lcr);
	
	dlm = 8 >> new_rate;
	
	sys_outb(base_addr + UART_DLM, lcr);
	
	dll = new_rate & 0xFF;
	
	sys_outb(base_addr + UART_DLL, lcr);
	
	lcr = lcr ^ DLAB;
	
	sys_outb(base_addr + UART_LCR, lcr);
	
	return 0;
}

uint8_t uart_get_polled(base_address base_addr, uint32_t *character){
	uint32_t st = 0;
	
	if( sys_inb(base_addr + UART_LSR, &st) != OK ) {
		return 1;
	} else {
		if ( st & (SER_OVERRUN_ERR | SER_PARITY_ERR	| SER_FRAME_ERR) ) {
			tickdelay(micros_to_ticks(DELAY_US));
			return 1;
		}
		else if(st & SER_REC_DATA){
			sys_inb(base_addr + UART_RBR, character);
			//printf("%c\n", &character);
			return 0;
		}
	}
	return 1;
}

uint8_t uart_get_polled_word(base_address base_addr, char **word){
	*word  = NULL;
	char *string = (char *)malloc(sizeof(char));
	string[0] = '\0';
	uint64_t i=0;
	uint32_t curr;
	do{
		if(uart_get_polled(base_addr,&curr)){
		}
		else{
			i++;
			string = realloc(string, (i+1) * sizeof(char));
			if(curr == '\0' || curr == '\n') curr = ' ';
			string[i-1] = curr;
			string[i] = '\0';
			if(curr == ' ')
				printf("%s\n", string);
		}
	}while(curr != '.');
	
	printf("%s\n", string);
	
	*word = malloc(i*sizeof(char));
	
	memcpy(*word, string, i);
	
	return 0;
}

uint8_t uart_send_polled(base_address base_addr, uint32_t character){
	uint32_t lsr = 0;
	do{
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(base_addr + UART_LSR, &lsr);
	}while(!(lsr & SER_THR_EMPTY));
	sys_outb(base_addr+UART_THR, character);
	return 0;
}

uint8_t uart_send_polled_word(base_address base_addr, char *word){
	uint64_t i=0;
	while(word[i] != '\0'){
		uart_send_polled(base_addr, word[i]);
		i++;
	}
	uart_send_polled(base_addr, '\0');	//force end of string
	return 0;
}
