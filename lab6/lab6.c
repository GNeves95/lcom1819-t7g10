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

#include "rtc.h"

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

//Due to this years version of lcf not having support for lab6, use this workaround
//Run the lab6 function from inside the lab1 functions called by lcf
int (timer_test_read_config)(uint8_t timer, enum timer_status_field field){
	printf("%d %d\n", timer, field);
	return rtc_test_conf();
}

int (timer_test_time_base)(uint8_t choice, uint32_t nope){
	printf("%d\n", nope);
	return rtc_test_date(choice);
}

int (timer_test_int)(uint8_t time){
	printf("%d\n", time);
	return rtc_test_int(time);
}

int (rtc_test_conf)(){
	//uint32_t data;
	for(int i=0; i < 14; i++){
		uint32_t data;
		sys_outb(RTC_ADDR_REG, i);
		sys_inb(RTC_DATA_REG, &data);
		switch(i){
			case 0:
				printf("Seconds: 0x%X %d\n", data, data);
				break;
			case 1:
				printf("Seconds Alarm: 0x%X %d\n", data, data);
				break;
			case 2:
				printf("Minutes: 0x%X %d\n", data, data);
				break;
			case 3:
				printf("Minutes Alarm: 0x%X %d\n", data, data);
				break;
			case 4:
				printf("Hours: 0x%X %d\n", data, data);
				break;
			case 5:
				printf("Hours Alarm: 0x%X %d\n", data, data);
				break;
			case 6:
				printf("Day Of The Week: 0x%X %d\n", data, data);
				break;
			case 7:
				printf("Day Of The Month: 0x%X %d\n", data, data);
				break;
			case 8:
				printf("Month: 0x%X %d\n", data, data);
				break;
			case 9:
				printf("Year: 0x%X %d\n", data, data);
				break;
			case 10:
				printf("A: 0x%X\n", data);
				break;
			case 11:
				printf("B: 0x%X\n", data);
				break;
			case 12:
				printf("C: 0x%X\n", data);
				break;
			case 13:
				printf("D: 0x%X\n", data);
				break;
			default:
				break;
		}
	}
	return 0;
}

int (rtc_test_date)(uint8_t choice){
	if(choice == 0){
		printf("RegB\n");
		uint32_t regB = 0;
		
		sys_outb(RTC_ADDR_REG, RTC_REG_B);
		sys_inb(RTC_DATA_REG, &regB);
		
		for(int i=0; i < 10; i++){
			uint32_t data;
			sys_outb(RTC_ADDR_REG, i);
			sys_inb(RTC_DATA_REG, &data);
			switch(i){
				case 0:
					printf("Seconds: 0x%X %d\n", data, data);
					break;
				case 2:
					printf("Minutes: 0x%X %d\n", data, data);
					break;
				case 4:
					printf("Hours: 0x%X %d\n", data, data);
					break;
				case 6:
					printf("Day Of The Week: 0x%X %d\n", data, data);
					break;
				case 7:
					printf("Day Of The Month: 0x%X %d\n", data, data);
					break;
				case 8:
					printf("Month: 0x%X %d\n", data, data);
					break;
				case 9:
					printf("Year: 0x%X %d\n", data, data);
					break;
			}
		}
		
		sys_outb(RTC_ADDR_REG, RTC_REG_B);
		sys_inb(RTC_DATA_REG, &regB);
		
		printf("0x%X\n", regB);
		
		if(regB & RTC_B_SET)
			regB = regB - RTC_B_SET;
		
		printf("0x%X\n", regB);
		
		sys_outb(RTC_ADDR_REG, RTC_REG_B);
		sys_outb(RTC_DATA_REG, regB);
		
		return 1;
	}
	else{
		printf("RegA\n");
		wait_valid_rtc();
		for(int i=0; i < 10; i++){
			uint32_t data;
			sys_outb(RTC_ADDR_REG, i);
			sys_inb(RTC_DATA_REG, &data);
			switch(i){
				case 0:
					printf("Seconds: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 2:
					printf("Minutes: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 4:
					printf("Hours: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 6:
					printf("Day Of The Week: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 7:
					printf("Day Of The Month: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 8:
					printf("Month: 0x%X %d\n", data, rtc_bcd_to_bin(data));
					break;
				case 9:
					printf("Year: 0x%X 20%d\n", data, rtc_bcd_to_bin(data));
					break;
			}
		}
	}
	return 0;
}

int (rtc_test_int)(uint8_t time){
	int estado;
	uint8_t bit_no;
	message msg;
	//int running = 1;
	int i=0;
	
	printf("BCD: %d\n", rtc_bcd());
	
	enable_rtc_int();
	
	disable_per_rtc_int();
	
	if(rtc_subscribe_int(&bit_no)!=0){
		return 1;
	}
	
	set_alarm(time);
	rtc_test_conf();
	
	while (i != -3)
	{
		driver_receive(ANY, &msg, &estado);
		if (is_ipc_notify(estado))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
				case HARDWARE:
					if (msg.m_notify.interrupts & BIT(bit_no)){
						if(rtc_ih() == 1){
							printf("Here!\n");
							i=-3;
						 }
						//i++;
					}
			}
		}
	}	
	
	printf("Alarmed\n");
	
	if(rtc_unsubscribe_int()!=0){
		return 1;
	}
	
	disable_rtc_int();
	
	timedate aux = get_date();
	
	printf("%02d:%02d:%02d, %s, %02d/%02d/%d, %d of %s of %d\n",aux.hours, aux.minutes, aux.seconds, aux.dweek, aux.day, aux.month, aux.year, aux.day, aux.mon, aux.year);
	
	return 0;
}
