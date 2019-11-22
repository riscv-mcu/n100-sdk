// See LICENSE file for licence details

#ifndef FUNC_H
#define FUNC_H

__BEGIN_DECLS

#include "drivers/timer.h"
#include "drivers/irqc.h"

void core_wfe();
void wfe();

void enable_mcycle_minstret();
void disable_mcycle_minstret();

void pmp_open_all_space();

void switch_m2u_mode();

uint32_t get_mtime_freq();

uint32_t mtime_lo(void);

// uint32_t mtime_hi(void);

uint32_t get_mtime_value();

uint32_t get_instret_value();

uint32_t get_cycle_value();

uint32_t get_cpu_freq();

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);

uint32_t get_timer_value();
uint32_t get_timer_freq();

///////////////////////////////////////////////////////////////////
/////// IRQC relevant functions
///////
void irqc_init ( uint32_t num_irq );

void irqc_enable_interrupt (uint32_t source);
void irqc_disable_interrupt (uint32_t source);

void irqc_set_pending(uint32_t source);
void irqc_clear_pending(uint32_t source);

// void irqc_set_intctrl (uint32_t source, uint8_t intctrl);
// uint8_t irqc_get_intctrl  (uint32_t source);

// void irqc_set_intattr (uint32_t source, uint8_t intattr);
// uint8_t irqc_get_intattr  (uint32_t source);

// void irqc_set_cliccfg (uint8_t cliccfg);
// uint8_t irqc_get_cliccfg ();

// void irqc_set_mth (uint8_t mth);
// uint8_t irqc_get_mth();

// //sets nlbits 
// void irqc_set_nlbits(uint8_t nlbits);


// //get nlbits 
// uint8_t irqc_get_nlbits();

// void irqc_set_irq_lvl(uint32_t source, uint8_t lvl);
// uint8_t irqc_get_irq_lvl(uint32_t source);

// void irqc_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs);
// uint8_t irqc_get_irq_lvl_abs(uint32_t source);

// void irqc_mode_enable();

// void irqc_set_vmode(uint32_t source);
// void irqc_set_nonvmode(uint32_t source);

void irqc_set_level_trig(uint32_t source);
void irqc_set_posedge_trig(uint32_t source);
void irqc_set_negedge_trig(uint32_t source);

__END_DECLS

#endif
