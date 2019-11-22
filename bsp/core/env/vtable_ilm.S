// See LICENSE for license details.

// See LICENSE for license details.

#include "drivers/riscv_encoding.h"

	.section .vtable_ilm


    .weak  irqc_msip_handler
    .weak  irqc_mtip_handler  
 
    .weak  irqc_irq3_handler

    .weak  irqc_irq4_handler
    .weak  irqc_irq5_handler
    .weak  irqc_irq6_handler
    .weak  irqc_irq7_handler

    .weak  irqc_irq8_handler
    .weak  irqc_irq9_handler
    .weak  irqc_irq10_handler
    .weak  irqc_irq11_handler

    .weak  irqc_irq12_handler
    .weak  irqc_irq13_handler
    .weak  irqc_irq14_handler
    .weak  irqc_irq15_handler

    .weak  irqc_irq16_handler
    .weak  irqc_irq17_handler
    .weak  irqc_irq18_handler
    .weak  irqc_irq19_handler

    .weak  irqc_irq20_handler
    .weak  irqc_irq21_handler
    .weak  irqc_irq22_handler
    .weak  irqc_irq23_handler

    .weak  irqc_irq24_handler
    .weak  irqc_irq25_handler
    .weak  irqc_irq26_handler
    .weak  irqc_irq27_handler

    .weak  irqc_irq28_handler
    .weak  irqc_irq29_handler
    .weak  irqc_irq30_handler
    .weak  irqc_irq31_handler

    .globl vector_base
vector_base:
    .word  irqc_msip_handler
    .word  irqc_mtip_handler  
    .word  0 
    .word  irqc_irq3_handler

    .word  irqc_irq4_handler
    .word  irqc_irq5_handler
    .word  irqc_irq6_handler
    .word  irqc_irq7_handler

    .word  irqc_irq8_handler
    .word  irqc_irq9_handler
    .word  irqc_irq10_handler
    .word  irqc_irq11_handler

    .word  irqc_irq12_handler
    .word  irqc_irq13_handler
    .word  irqc_irq14_handler
    .word  irqc_irq15_handler

    .word  irqc_irq16_handler
    .word  irqc_irq17_handler
    .word  irqc_irq18_handler
    .word  irqc_irq19_handler

    .word  irqc_irq20_handler
    .word  irqc_irq21_handler
    .word  irqc_irq22_handler
    .word  irqc_irq23_handler

    .word  irqc_irq24_handler
    .word  irqc_irq25_handler
    .word  irqc_irq26_handler
    .word  irqc_irq27_handler

    .word  irqc_irq28_handler
    .word  irqc_irq29_handler
    .word  irqc_irq30_handler
    .word  irqc_irq31_handler

    j trap_entry
    .align 2



