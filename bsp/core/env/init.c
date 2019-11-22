//See LICENSE for license details.
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "drivers/riscv_encoding.h"
#include "drivers/func.h"


  // Need to include the soc_init function, so need to include the soc_func.h here
#include "soc/drivers/soc_func.h"


void _init()
{
  #ifndef NO_INIT
  soc_init();// We must put this first before use any printf later

  printf("**************************************\n");
  printf("**************************************\n");
  printf("*                                    *\n");
  printf("Core freq at %d Hz\n", get_cpu_freq());
  printf("*                                    *\n");
  printf("**************************************\n");
  printf("**************************************\n");

  //IRQC init 
  irqc_init(IRQC_NUM_INTERRUPTS);

  //
  /* Before enter into main, add the cycle/instret disable by default to save power,
       only use them when needed to measure the cycle/instret */
  disable_mcycle_minstret();

  #endif
}

void _fini()
{
}
