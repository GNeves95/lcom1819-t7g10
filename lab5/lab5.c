// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

//#include <lcom/vbe.h>

#include <lcom/video_gr.h>

//#include <liblm.h>

#include <stdint.h>
#include <stdio.h>

#include "kbd.h"
#include "i8042.h"
#include "vga.h"
//#include "timer.h"
#include "i8254.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {
  /* To be completed */
  //printf("%s(0x%03x, %u): under construction\n", __func__, mode, delay);
  
  char *vm = (char*)vg_init(mode);
  
  sleep(delay);
  
  vg_exit();
  
  printf("%x\n",vm);

  return 1;
}	

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                       uint16_t width, uint16_t height, uint32_t color) {
  /* To be completed */
  printf("%s(0x%03X, %u, %u, %u, %u, 0x%08x): under construction\n",
         __func__, mode, x, y, width, height, color);
         
  int estado;
  uint8_t bit_no;
  message msg;
  bool running = true;

  if (kbd_subscribe_int(&bit_no) != 0)
  {
    printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
    return 1;
  };
         
  char *vm = (char*)vg_init(mode);
  
  vg_draw_rectangle(x, y, width, height, color);

  while (running)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no)
        {
          kbc_ih();
          bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_BREAK)
          { //Exists upon the release of ESC key
            running = false;
          }
        }
        break;
      default:
        break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0)
    return 2;
  
  vg_exit();
  
  printf("%x\n",vm);

  return 1;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__, mode, no_rectangles, first, step);
  int estado;
  uint8_t bit_no;
  message msg;
  bool running = true;

  if (kbd_subscribe_int(&bit_no) != 0)
  {
    printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
    return 1;
  };
  
  char *vm = (char*)vg_init(mode);
  int width = getHRes()/no_rectangles;
  int height = getVRes()/no_rectangles;
  
  printf("%d %d %d %d %d\n", no_rectangles, getHRes(), width, getVRes(), height);
  
  uint32_t color = first;
  //vg_draw_rectangle(0,0,getHRes(),getVRes(),1);
  
  for(unsigned int i = 0; i < no_rectangles; i++){
	  for(unsigned int j = 0; j <= no_rectangles; j++){
		  if(getcm() == indexed){
			  //index(row, col) = (first + (row * no_rectangles + col) * step) % (1 << BitsPerPixel)
			  color = (first + (i * no_rectangles + j) * step) % (1 << getBitsPerPixel());
			  vg_draw_rectangle(width*j,height*i,width,height,color);
		  }
		  else{
			  /*
			   * R(row, col) = (R(first) + col * step) % (1 << RedScreeMask)
			   * G(row, col) = (G(first) + row * step) % (1 << GreenScreeMask)
			   * B(row, col) = (B(first) + (col + row) * step) % (1 << BlueScreeMask)
			   * */
			   color = (first) + ((j * step) % (1 << getRSM())) + ((i * step) % (1 << getGSM())) + (((i + j) * step) % (1 << getBSM()));
			   printf("%l\n", color);
			   printf("%x\n", color);
			   vg_draw_rectangle(width*j,height*i,width,height,color);
		  }
	  }
  }
  
  //vg_draw_rectangle(x, y, width, height, color);

  while (running)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no)
        {
          kbc_ih();
          bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_BREAK)
          { //Exists upon the release of ESC key
            running = false;
          }
        }
        break;
      default:
        break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0)
    return 2;
  
  vg_exit();
  
  printf("%x\n",vm);

  return 1;
}

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  int estado;
  uint8_t bit_no;
  message msg;
  bool running = true;

  if (kbd_subscribe_int(&bit_no) != 0)
  {
    printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
    return 1;
  };
         
  char *vm = (char*)vg_init(0x105);
  
  drawXPM(xpm, x, y);

  while (running)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & bit_no)
        {
          kbc_ih();
          bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_BREAK)
          { //Exists upon the release of ESC key
            running = false;
          }
        }
        break;
      default:
        break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0)
    return 2;
  
  vg_exit();
  
  printf("%x\n",vm);

  return 1;
}

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n", __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  int estado;
  uint8_t kbd_no, tmr_no;
  message msg;
  bool running = true;
  int counter = 0;
  int length = 0;
  int mov = 0;
  int refresh = 1;
  bool horizontal = true;
  
  if(xi != xf){
	  horizontal = true;
	  length = xf - xi;
  }
  else if(yi != yf){
	  horizontal = false;
	  length = yf - yi;
  }
  if(speed > 0){
	  mov = speed;
  }
  else if(speed < 0){
	  refresh = -speed;
	  mov = 1;
  }

  kbd_subscribe_int(&kbd_no);

  timer_subscribe_int(&tmr_no);
         
  char *vm = (char*)vg_init(0x105);
  
  unsigned int x = xi;
  unsigned int y = yi;
  
  drawXPM(xpm, x, y);

  while (running)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & kbd_no)
        {
          kbc_ih();
          bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_BREAK)
          { //Exists upon the release of ESC key
            running = false;
          }
        }
         if (msg.m_notify.interrupts & tmr_no)
        {
			counter++;
			if((counter*fr_rate) % 60 == 0){
				if(((counter*fr_rate) / 60)%refresh == 0){
					clearScreenDouble();
					drawXPMdouble(xpm, x, y);
					swap();
					if(horizontal == true){
						if(x+mov > xf) x = xf;
						else x += mov;
					}
					else{
						if(y+mov > yf) y = yf;
						else y += mov;
					}
				}
			}
		}
        break;
      default:
        break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0)
    return 2;
    
  if(timer_unsubscribe_int() != 0)
	return 3;
  
  vg_exit();
  
  printf("%x\n",vm);

  return 1;
}

int (video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);
  
  //vg_init(0x105);

  vg_vbe_contr_info_t info_p;
 // memset(&info_p, 0, sizeof(vg_vbe_contr_info_t));
  
  /*info_p.OEMString = malloc(20*sizeof(char));
  info_p.VideoModeList = malloc(99*sizeof(u_int16_t));
  info_p.OEMVendorNamePtr = malloc(20*sizeof(char));
  info_p.OEMProductNamePtr  = malloc(20*sizeof(char));
  info_p.OEMProductRevPtr = malloc(20*sizeof(char));*/
  
  /*strcpy(info_p.OEMString, "Forca Guilherme!");
  *(info_p.VideoModeList) = 0;
  strcpy(info_p.OEMVendorNamePtr, "Forca Guilherme!");
  strcpy(info_p.OEMProductNamePtr, "Forca Guilherme!");
  strcpy(info_p.OEMProductRevPtr, "Forca Guilherme!");*/
  //info_p.OEMString = "Forca Guilherme!"
  
  if(vg_vbe_get_info(&info_p) != 0){
	printf("Error getting controller info!\n");
	return 2;
  }
  
  //vg_exit();
  
  vg_display_vbe_contr_info(&info_p);
  
  return 1;
}  

