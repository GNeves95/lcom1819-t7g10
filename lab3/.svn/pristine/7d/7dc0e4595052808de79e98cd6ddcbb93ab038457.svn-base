// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "kbd.h"
#include "i8042.h"
#include <lcom/timer.h>
#include <stdlib.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasksdriver_receive
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)(bool assembly)
{
  int estado;
  uint8_t bit_no;
  message msg;
  bool running = true;

  if (kbd_subscribe_int(&bit_no) != 0)
  {
    printf("kbd_subscribe_int(34): Couldn't subscribe to keyboard interrupts\n");
    return 1;
  };

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
          (assembly) ? kbc_asm_ih() : kbc_ih();
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
  return 0;
}

int(kbd_test_poll)()
{
  pret.receive = false;

  if (sys_outb(KBD_CTRL, DIS_KBD_INT) != 0)
  {
    printf("kbd_test_poll(91): couldn't disable kb interrupts\n");
    return 1;
  }

  int flag = 0;

  while (flag != 1)
  {
    kbd_scan_loop();
    bool msb = (pret.kbc_code & BIT(7)) ? true : false;
    uint8_t kbc_code_bytes[3];

    kbc_code_bytes[0] = (pret.kbc_code >> 0) & 0xFF;
    kbc_code_bytes[1] = (pret.kbc_code >> 8) & 0xFF;
    kbc_code_bytes[2] = (pret.kbc_code >> 16) & 0xFF;

    kbd_print_scancode(msb, 1, kbc_code_bytes);
    if (kbc_code_bytes[0] == ESC_BREAK)
    { //Exists upon the release of ESC key
      flag = 1;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    printf("\n");
  }

  if (sys_outb(KBD_CTRL, EN_KBD_INT) != 0)
  {
    printf("kbd_test_poll(116): couldn't reenable kb interrupts\n");
    return 2;
  }
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n)
{
  int state, timer = 0, flag = 0;
  message msg;
  uint8_t bit_no_tmr;
  uint8_t bit_no_kbd;
  kbd_subscribe_int(&bit_no_kbd);
  timer_subscribe_int(&bit_no_tmr);
  while (flag != 1 && timer < 60 * n)
  {
    driver_receive(ANY, &msg, &state);
    if (is_ipc_notify(state))
    {
      switch (_ENDPOINT_P(msg.m_source))
      {

      case HARDWARE:

        if (msg.m_notify.interrupts & bit_no_kbd)
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
            flag = 1;
          }
          timer = 0;
          tickdelay(micros_to_ticks(DELAY_US));
          printf("\n");
        }
        if (msg.m_notify.interrupts & bit_no_tmr)
        {
          timer++;
          if (timer % 60 == 0)
            printf("Seconds: %d of %d\n", (timer / 60) + 1, n);
          timer_int_handler();
        }
        break;
      default:
        break;
      }
    }
  }
  if (timer >= 60 * n)
    printf("Timeout\n");
  if (kbd_unsubscribe_int() != 0 && timer_unsubscribe_int() != 0)
    return 1;
  return 0;
}
