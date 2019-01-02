// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "kbd.h"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "ps2.h"
#include "vga.h"
#include "game.h"
#include "bmp.h"

#include "img/floor.xpm"
#include "img/souto.xpm"
#include "img/zombie.xpm"
#include "img/mouse.xpm"

#include <sys/types.h> 
#include <unistd.h> 

bool printZombie = false, printFloor = false;

xpm_image_t zombiexpm, floorxpm;

void drawBackground(){
	clearScreenDouble();
	if(printFloor){
		for(int i=0; i < ((getHRes()/200)+2); i++){
			for(int j=0; j < ((getVRes()/200)+1); j++){
				int xPos = ((i*200)+souto->x)-(getHRes()/2);
				int yPos = ((j*200)+souto->y)-(getVRes()/2);
				if((xPos >= 0) && (yPos >= 0) && (xPos < 2000) && (yPos < 2000)){
					drawNewXPM(floorxpm,(((i*200)/*-(getHRes()/2)*/) - (xPos%200)),(((j*200)/*-(getVRes()/2)*/) - (yPos%200)));
				}
				else set_pixel_double(i,j,0x000000);
			}
		}
	}
	else{
		for(int i=0; i < getHRes(); i++){
			for(int j=0; j < getVRes(); j++){
				if(((i+souto->x)-(getHRes()/2) > 0) && ((j+souto->y)-(getVRes()/2) > 0) && ((i+souto->x)-(getHRes()/2) < 2000) && ((j+souto->y)-(getVRes()/2) < 2000)){
					if(((i+souto->x)-(getHRes()/2))%100 == 0 || ((j+souto->y)-(getVRes()/2))%100 == 0){
						set_pixel_double(i,j,0xFFFFFF);
					}
					else set_pixel_double(i,j,0x0000FF);
				}
				else set_pixel_double(i,j,0x000000);
			}
		}
	}
}

void chaseHero(zombie *student){
	if(student->x < souto->x && student->speed_x < 5){
		student->speed_x+=1;
	}
	else if(student->x > souto->x && student->speed_x > -5){
		student->speed_x-=1;
	}
	else if(student->x == souto->x){
		student->speed_x = 0;
	}
	
	if(student->y < souto->y && student->speed_y < 5){
		student->speed_y+=1;
	}
	else if(student->y > souto->y && student->speed_y > -5){
		student->speed_y-=1;
	}
	else if(student->y == souto->y){
		student->speed_y = 0;
	}
	if(student->speed_x > 5) student->speed_x = 5;
	if(student->speed_x < -5) student->speed_x = -5;
	if(student->speed_y > 5) student->speed_y = 5;
	if(student->speed_y < -5) student->speed_y = -5;
	
	if(colision(student->x + student->speed_x, student->y + student->speed_y) != (int)student->id && colision(student->x + student->speed_x, student->y + student->speed_y) != -1){
		student->x = student->x;
		student->y = student->y;
	}
	else{
		student->x += student->speed_x;
		student->y += student->speed_y;
	}
	
	if(student->x < 0) student->x = 0;
	else if(student->x > 2000) student->x = 2000;
	if(student->y < 0) student->y = 0;
	else if(student->y > 2000) student->y = 2000;
}

void drawZombie(zombie *student){
	int difX = student->x - souto->x;
	int difY = student->y - souto->y;
	
	if(mod(difX)-100 < getHRes()/2 && mod(difY)-92 < getVRes()/2){
		if(printZombie == false)vg_draw_rectangle_double((getHRes()/2)+difX, (getVRes()/2)+difY, 100, 92, 0xFF0000);
		else drawNewXPM(zombiexpm, (getHRes()/2)+difX-50, (getVRes()/2)+difY-47);
	}
}

int handleEnemies(){
	zombie * curr = alunos->first;
	for(unsigned int i=0; i < alunos->size; i++){
		if(!(curr->alive)){
			break;
		}
		chaseHero(curr);
		if(colision(curr->x, curr->y) == -1) return 1;
		drawZombie(curr);
		curr = curr->next;
	}
	return 0;
}

void keyHandle(){
	bool msb = (kbc_code & BIT(7)) ? true : false;

          uint8_t kbc_code_bytes[3];

          kbc_code_bytes[0] = (kbc_code >> 0) & 0xFF;
          kbc_code_bytes[1] = (kbc_code >> 8) & 0xFF;
          kbc_code_bytes[2] = (kbc_code >> 16) & 0xFF;

          kbd_print_scancode(msb, 1, kbc_code_bytes);
          if (kbc_code_bytes[0] == ESC_MAKE)
          {
			  ks.esc = true;
          }
          if (kbc_code_bytes[0] == ESC_BREAK)
          {
			  ks.esc = false;
          }
          if (kbc_code_bytes[0] == ONE_MAKE)
          {
			  ks.numb1 = true;
		  }
          if (kbc_code_bytes[0] == ONE_BREAK)
          {
			  ks.numb1 = false;
		  }
          if (kbc_code_bytes[0] == TWO_MAKE)
          {
			  ks.numb2 = true;
		  }
          if (kbc_code_bytes[0] == TWO_BREAK)
          {
			  ks.numb2 = false;
		  }
          if (kbc_code_bytes[0] == THREE_MAKE)
          {
			  ks.numb3 = true;
		  }
          if (kbc_code_bytes[0] == THREE_BREAK)
          {
			  ks.numb3 = false;
		  }
          if (kbc_code_bytes[0] == FOUR_MAKE)
          {
			  ks.numb4 = true;
		  }
          if (kbc_code_bytes[0] == FOUR_BREAK)
          {
			  ks.numb4 = false;
		  }
          if (kbc_code_bytes[0] == FIVE_MAKE)
          {
			  ks.numb5 = true;
		  }
          if (kbc_code_bytes[0] == FIVE_BREAK)
          {
			  ks.numb5 = false;
		  }
          if (kbc_code_bytes[0] == W_MAKE)
          {
			  ks.w = true;
		  }
          if (kbc_code_bytes[0] == W_BREAK)
          {
			  ks.w = false;
		  }
          if (kbc_code_bytes[0] == S_MAKE)
          {
			  ks.s = true;
		  }
          if (kbc_code_bytes[0] == S_BREAK)
          {
			  ks.s = false;
		  }
          if (kbc_code_bytes[0] == A_MAKE)
          {
			  ks.a = true;
		  }
          if (kbc_code_bytes[0] == A_BREAK)
          {
			  ks.a = false;
		  }
          if (kbc_code_bytes[0] == D_MAKE)
          {
			  ks.d = true;
		  }
          if (kbc_code_bytes[0] == D_BREAK)
          {
			  ks.d = false;
		  }
}

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  /* To be completed */
  printf("%s(%d, %p): under construction\n", __func__, argc, argv);
  
  srand(time(0));

  int estado, fr_rate=60;
  uint8_t kbd_bit, mouse_bit, tmr_bit;
  uint16_t x=0, y=0;
  message msg;
  bool running = true;
  bool lb = false, rb=false;
  
  //int sx=0, sy=0, px=0,py=0;
  
  gameInit();

  if (kbd_subscribe_int(&kbd_bit) != 0)
  {
    printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
    return 1;
  }
  
  timer_subscribe_int(&tmr_bit);
  
  if(init_mouse(&mouse_bit)){
		printf("mouse_test_packet(46): problem initializing mouse!\n");
		return 1;
  }
  
  char *vm = (char*)vg_init(0x115);
  
  bool printSouto = false, printMouse = false;
  
  Bitmap *floor = parseBmp("/home/lcom/labs/proj/img/floor.bmp");
  if(floor == NULL){
	  printf("%s: failed loading floor.bmp\n", __func__);
	  printFloor = false;
  }
  
  xpm_image_t soutoxpm, mousexpm;
  
  if (xpm_load(floor_xpm, XPM_8_8_8, &floorxpm) != NULL){
	  printFloor = true;
  }
  
  if (xpm_load(souto_xpm, XPM_8_8_8, &soutoxpm) != NULL){
	  printSouto = true;
  }
  
  if (xpm_load(zombie_xpm, XPM_8_8_8, &zombiexpm) != NULL){
	  printZombie = true;
  }
  
  if (xpm_load(mouse_xpm, XPM_8_8_8, &mousexpm) != NULL){
	  printMouse = true;
  }
  
  while (running)
  {
    driver_receive(ANY, &msg, &estado);
    if (is_ipc_notify(estado))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE:
        if (msg.m_notify.interrupts & BIT(mouse_bit)){
			mouse_ih();
						
			if(counter == -1)
				break;
							
			if(counter > 2) {
				//struct packet pp;
							
				/*pp.bytes[0] = paux->bytes[0];
				pp.bytes[1] = paux->bytes[1];
				pp.bytes[2] = paux->bytes[2];
				pp.rb = paux->rb;
				pp.mb = paux->mb;
				pp.lb = paux->lb;
				pp.delta_x = paux->delta_x;
				pp.delta_y = paux->delta_y;
				pp.x_ov = paux->x_ov;
				pp.y_ov = paux->y_ov;*/
				
				if(paux->rb)rb=true;
				else if(!(paux->rb)) rb=false;
				if(paux->lb)lb=true;
				else if(!(paux->lb)) lb=false;
				
				if(!(paux->x_ov))x+=paux->delta_x;
				if(!(paux->y_ov))y-=paux->delta_y;
				
				if(x < 0) x=0;
				else if(x>getHRes()) x= getHRes();
				
				if(y < 0) y=0;
				else if(y>getVRes()) y= getVRes();
							
				//mouse_print_packet(&pp);
				//int_cnt++;
				counter = 0;
			}
		}
        if (msg.m_notify.interrupts & BIT(kbd_bit))
        {
          kbc_ih();
          keyHandle();
          
          if (ks.esc)
          { //Exists upon the release of ESC key
            running = false;
          }
          
          if (ks.numb1)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x10F);
		  }
          if (ks.numb2)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x112);
		  }
          if (ks.numb3)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x115);
		  }
          if (ks.numb4)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x118);
		  }
          if (ks.numb5)
          {
			  vg_exit();
  
			  printf("%x\n",vm);
			  
			  vm = (char*)vg_init(0x11B);
		  }
          
        }
        if (msg.m_notify.interrupts & BIT(tmr_bit))
        {
			//cnt++;
			timer_int_handler();
			//printf("after ih\n");
			if((cont*fr_rate) % 60 == 0){
				//printf("frame rate\n");
				clearScreenDouble();
				//printf("clear screen\n");
				if(ks.s && souto->y < 2000){
					if(souto->speed_y < 5) souto->speed_y++;
					souto->y+=souto->speed_y;
				}
				if(ks.w && souto->y > 0){
					if(souto->speed_y > -5) souto->speed_y--;
					souto->y+=souto->speed_y;
				}
				if(!ks.s && souto->speed_y > 0){
					souto->speed_y--;
					souto->y+=souto->speed_y;
				}
				if(!ks.w && souto->speed_y < 0){
					souto->speed_y++;
					souto->y+=souto->speed_y;
				}
				if(ks.d && souto->x < 2000){
					if(souto->speed_x < 5) souto->speed_x++;
					souto->x+=souto->speed_x;
				}
				if(ks.a && souto->x > 0){
					if(souto->speed_x > -5) souto->speed_x--;
					souto->x+=souto->speed_x;
				}
				if(!ks.d && souto->speed_x > 0){
					souto->speed_x--;
					souto->x+=souto->speed_x;
				}
				if(!ks.a && souto->speed_x < 0){
					souto->speed_x++;
					souto->x+=souto->speed_x;
				}
				
				if(souto->x < 0) souto->x = 0;
				else if(souto->x > 2000) souto->x = 2000;
				if(souto->y < 0) souto->y = 0;
				else if(souto->y > 2000) souto->y = 2000;
				
				//printf("spped\n");
				
				/*if(printFloor) drawNewXPM(floorxpm, 200, 200);
				
				else drawBackground();*/
				
				//printf("background\n");
				drawBackground();
				
				//drawXPMdouble(floor_xpm, 220, 220);
				//printf("handle\n");
				if(handleEnemies()==1) running = false;
				
				//printf("before souto\n");
				if(printSouto == true){
					drawNewXPM(soutoxpm, (getHRes()/2)-50, (getVRes()/2)-33);
				}
				else vg_draw_rectangle_double((getHRes()/2)-50, (getVRes()/2)-33, 100, 66, 0x00FF00);
				//printf("after souto\n");
				
				if(printMouse){
					drawNewXPM(mousexpm, x-12, y-12);
				}
				else vg_draw_rectangle_double(x-12, y-12, 25, 25, 0xFFFFFF);
				printf("after mouse\n");
				
				if(lb == true){
					printf("click\n");
					int mouseX = souto->x - getHRes()/2 + x;
					int mouseY = souto->y - getVRes()/2 + y;
					if(mouseX > 0 && mouseX < 2000 && mouseY > 0 && mouseY < 2000){
						printf("inside map\n");
						int zomb = colision(mouseX, mouseY);
						printf("after colision %d\n", zomb);
						if(zomb > 0){
							printf("zombie exists %d\n", zomb);
							killZombie((unsigned int) zomb);
							printf("killed zombie\n");
						}
					}
				}
				
				swap();
				printf("after swap\n");
				//if(((cont*fr_rate) / 3600) == 5) running = false;
				
				if(cont*fr_rate % 3600 == 0){
					spawnZombie();
					cont = 0;
				}
			}
		}
        break;
      default:
        break;
      }
    }
  }  
  
  vg_exit();
  
  if(printFloor){
	  free(floor->bmpData);
	  free(floor);
  }
  
  printf("%x\n",vm);

  if (kbd_unsubscribe_int() != 0)
    return 2;
    
  if(timer_unsubscribe_int() != 0)
	return 3;
	
  if (ps2_unsubscribe_int() != 0)
	return 2;
	
  if(ps2_ignore() != 0)
  {
	printf("mouse_test_packet(53): Failed ignoring ps2 buffer!\n");
	return 3;
  }

  return 1;
}
