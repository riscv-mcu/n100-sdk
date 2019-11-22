// See LICENSE file for licence details

#ifndef IRQC_H
#define IRQC_H

  // Need to know the following info from the soc.h, so include soc.h here
  //   SOC_IRQC_CTRL_ADDR      : what is the base address of IRQC in this SoC
  //   SOC_IRQC_NUM_INTERRUPTS : how much of irq configured in total for the IRQC in this SoC
#include "soc/drivers/soc.h"

#define IRQC_NUM_INTERRUPTS (SOC_IRQC_NUM_INTERRUPTS + 3)

#define IRQC_CFG_NLBITS_MASK          _AC(0x1E,UL)
#define IRQC_CFG_NLBITS_LSB     (1u)

#define IRQC_INT_MSIP          0
#define IRQC_INT_MTIP          1


#define MTIME_HANDLER   irqc_mtip_handler 
#define MSIP_HANDLER    irqc_msip_handler



#endif

