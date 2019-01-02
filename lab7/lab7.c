// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

//#include <lcom/vbe.h>

#include <lcom/video_gr.h>

//#include <liblm.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include "uart.h"

// Any header files included below this line should have been created by you

int rtc_test_conf();
int rtc_test_date(uint8_t choice);
int rtc_test_int(uint8_t time);

int main(int argc, char **argv)
{
	// sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab6/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab6/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
}

uint8_t str_contains(char *str, char elem){
	while(str[0] != '\0'){
		if(str[0] == elem) return 1;
		str++;
	}
	return 0;
}

int ser_test_conf(unsigned short base_addr) {
    /* To be completed */
    printf("0x%X\n",base_addr);
    
    //base_address aux = COM1;
	
	uart_get_conf(COM1);
    
    return 0;
}

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	           long parity, unsigned long rate) { 
    /* To be completed */
    printf("0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n",base_addr, bits, stop, parity, rate);
    
    uart_set(COM1, bits, stop, parity, rate);
    
    return 0;
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) {
    /* To be completed */
    printf("0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, %d, \n",base_addr, tx, bits, stop, parity, rate, stringc );
    
    uart_set(COM1, bits, stop, parity, rate);
    
    if(strings == NULL){
		printf("Strings are NULL\n");
	}
    
    if(tx == '0'){
		char *aux = "";
		uart_get_polled_word(COM1, &aux);
	}
	if(tx == '1'){
		char *aux = "yolo swag\ngangster yaaaaasss.\0";
		do{
			char *aux = "yolo swag\ngangster yaaaaasss.\0";
			uart_send_polled_word(COM1, aux);
		}while(!str_contains(aux, '.'));
	}
    
    return 0;
}

int ser_test_int(/* details to be provided */) { 
    /* To be completed */
    
    return 0;
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
    
    return 0;
}

//Due to this years version of lcf not having support for lab6, use this workaround
//Run the lab7 function from inside the lab1 functions called by lcf
int (timer_test_read_config)(uint8_t timer, enum timer_status_field field){
	printf("%d %d\n", timer, field);
	return ser_test_conf(timer);
}

int (timer_test_time_base)(uint8_t choice, uint32_t nope){
	printf("%d %d\n", choice, nope);
	ser_test_set(1, 8, 2, 1, 115200);
	return uart_get_conf(COM1);
}

int (timer_test_int)(uint8_t time){
	printf("%d\n", time);
	//printf("a\n");
	char *strings = (char*)malloc(sizeof(char));
	strings[0] = '\0';
	if(time == 0) return ser_test_poll(1, '0', 8, 2, 1, 115200, 1, &strings);
	if(time == 1) return ser_test_poll(1, '1', 8, 2, 1, 115200, 1, &strings);
	return uart_get_conf(COM1);
}

