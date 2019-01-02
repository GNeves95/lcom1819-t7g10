// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "ps2.h"
#include "mouse.h"
#include "i8042.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, cnt);
    
    int estado;
	uint8_t bit_no;
	message msg;
	uint32_t int_cnt = 0;
	
	if(init_mouse(&bit_no)){
		printf("mouse_test_packet(46): problem initializing mouse!\n");
		return 1;
	}
	
	//ps2_ignore();

	while (int_cnt < cnt){
		driver_receive(ANY, &msg, &estado);
		if (is_ipc_notify(estado)){
			switch (_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if (msg.m_notify.interrupts & BIT(bit_no)){
						mouse_ih();
						
						if(counter == -1)
							break;
							
						if(counter > 2) {
							struct packet pp;
							
							pp.bytes[0] = paux->bytes[0];
							pp.bytes[1] = paux->bytes[1];
							pp.bytes[2] = paux->bytes[2];
							pp.rb = paux->rb;
							pp.mb = paux->mb;
							pp.lb = paux->lb;
							pp.delta_x = paux->delta_x;
							pp.delta_y = paux->delta_y;
							pp.x_ov = paux->x_ov;
							pp.y_ov = paux->y_ov;
							
							mouse_print_packet(&pp);
							int_cnt++;
							counter = 0;
						}
						/*printf("Got interrupt!\n");
						int_cnt++;*/
					}
					break;
				default:
					break;
			}
		}
	}

	if (ps2_unsubscribe_int() != 0)
		return 2;
	
	if(ps2_ignore() != 0)
	{
		printf("mouse_test_packet(53): Failed ignoring ps2 buffer!\n");
		return 3;
	}
	
	return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    
    
    
    return 1;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, x_len, tolerance);
    return 1;
}
