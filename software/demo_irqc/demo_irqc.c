// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "stdatomic.h"

#include "drivers/func.h"
#include "soc/drivers/soc.h"
#include "soc/drivers/board.h"
#include "drivers/riscv_encoding.h"
#include "drivers/timer.h"
#include "drivers/irqc.h"

#define BUTTON_1_GPIO_OFFSET 0
#define BUTTON_2_GPIO_OFFSET 1

#define IRQC_INT_DEVICE_BUTTON_1 (SOC_IRQC_INT_GPIO_BASE + BUTTON_1_GPIO_OFFSET)
#define IRQC_INT_DEVICE_BUTTON_2 (SOC_IRQC_INT_GPIO_BASE + BUTTON_2_GPIO_OFFSET)
// The real value is:
//#define IRQC_INT_DEVICE_BUTTON_1 3 // 0+3
//#define IRQC_INT_DEVICE_BUTTON_2 4 // 1+3


// Since the BUTTON_1 IRQC IRQ number is 3, and BUTTON_2 is 4, we need to overriede the irq3/4 handler 
#define BUTTON_2_HANDLER irqc_irq4_handler
#define BUTTON_1_HANDLER irqc_irq3_handler



void wait_seconds(size_t n)
{
  unsigned long start_mtime, delta_mtime;

  // Don't start measuruing until we see an mtime tick
  unsigned long tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < (n * TIMER_FREQ));

  printf("-----------------Waited %d seconds.\n", n);
}


static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}

int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}

char * read_instructions_msg= " \
\n\
 ";


const char * printf_instructions_msg= " \
\n\
\n\
\n\
\n\
This is printf function printed:  \n\
\n\
             !! Here We Go, Nuclei-N100 !! \n\
\n\
     ######    ###    #####   #####          #     #\n\
     #     #    #    #     # #     #         #     #\n\
     #     #    #    #       #               #     #\n\
     ######     #     #####  #        #####  #     #\n\
     #   #      #          # #                #   #\n\
     #    #     #    #     # #     #           # #\n\
     #     #   ###    #####   #####             #\n\
\n\
 ";



void __attribute__ ((interrupt)) BUTTON_1_HANDLER(void) {
 
  #ifdef CFG_SIMULATION
  write (STDOUT_FILENO, "----Begin button1 handler\n", strlen("----Begin button1 handler\n"));
  #else
  printf ("%s","----Begin button1 handler----Vector mode\n");
  #endif

  // Green LED On
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << GREEN_LED_GPIO_OFFSET);

  // Clear the GPIO Pending interrupt by writing 1.
  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_1_GPIO_OFFSET);

  #ifdef CFG_SIMULATION
  write (STDOUT_FILENO, "----End button1 handler\n", strlen("----End button1 handler\n"));
  #else
  wait_seconds(2);// Wait for a while

  printf ("%s","----End button1 handler\n");

  #endif
  
};


void __attribute__ ((interrupt)) BUTTON_2_HANDLER(void) {

  #ifdef CFG_SIMULATION
  write (STDOUT_FILENO, "----------Begin button2 handler\n", strlen("----------Begin button2 handler\n"));
  #else
  printf ("%s","--------Begin button2 handler----Vector mode\n");
  #endif

  // Blue LED On
  GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << BLUE_LED_GPIO_OFFSET);

  GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_2_GPIO_OFFSET);

  #ifdef CFG_SIMULATION
  write (STDOUT_FILENO, "----------End button2 handler\n", strlen("----------End button2 handler\n"));
  #else
  wait_seconds(2);// Wait for a while

  printf ("%s","--------End button2 handler\n");
  #endif

};

void config_irqc_irqs (){

  // Have to enable the interrupt both at the GPIO level,
  // and at the IRQC level.
  irqc_enable_interrupt (IRQC_INT_DEVICE_BUTTON_1);
  irqc_enable_interrupt (IRQC_INT_DEVICE_BUTTON_2);
;
 } 


int main(int argc, char **argv)
{
  // Set up the GPIOs such that the LED GPIO
  // can be used as both Inputs and Outputs.
  

  GPIO_REG(GPIO_OUTPUT_EN)  &= ~((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));
  GPIO_REG(GPIO_PULLUP_EN)  &= ~((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));
  GPIO_REG(GPIO_INPUT_EN)   |=  ((0x1 << BUTTON_1_GPIO_OFFSET) | (0x1 << BUTTON_2_GPIO_OFFSET));

  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_1_GPIO_OFFSET);
  GPIO_REG(GPIO_RISE_IE) |= (1 << BUTTON_2_GPIO_OFFSET);


  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< RED_LED_GPIO_OFFSET) | (0x1<< GREEN_LED_GPIO_OFFSET) | (0x1 << BLUE_LED_GPIO_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< RED_LED_GPIO_OFFSET)| (0x1<< GREEN_LED_GPIO_OFFSET) | (0x1 << BLUE_LED_GPIO_OFFSET)) ;

  GPIO_REG(GPIO_OUTPUT_VAL)  |=   (0x1 << RED_LED_GPIO_OFFSET) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1<< BLUE_LED_GPIO_OFFSET) | (0x1<< GREEN_LED_GPIO_OFFSET)) ;


  
  // Print the message
  printf ("%s",printf_instructions_msg);

  //printf("\nIn main function, the mstatus is 0x%x\n", read_csr(mstatus));

  printf ("%s","\nThank you for supporting RISC-V, you will see the blink soon on the board!\n");

 
  config_irqc_irqs();

  // Enable interrupts in general.
  set_csr(mstatus, MSTATUS_MIE);


  // For Bit-banging 
  
  uint32_t bitbang_mask = 0;
  bitbang_mask = (1 << 13);

  GPIO_REG(GPIO_OUTPUT_EN) |= bitbang_mask;

  
  while (1){
    GPIO_REG(GPIO_OUTPUT_VAL) ^= bitbang_mask;

    // For Bit-banging with Atomics demo if the A extension is supported.
    //atomic_fetch_xor_explicit(&GPIO_REG(GPIO_OUTPUT_VAL), bitbang_mask, memory_order_relaxed);
  }

  return 0;

}
