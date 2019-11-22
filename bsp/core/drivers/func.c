// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "drivers/riscv_encoding.h"
#include "drivers/func.h"

    // Configure PMP to make all the address space accesable and executable
void pmp_open_all_space(){
    // Config entry0 addr to all 1s to make the range cover all space
    asm volatile ("li x6, 0xffffffff":::"x6");
    asm volatile ("csrw pmpaddr0, x6":::);
    // Config entry0 cfg to make it NAPOT address mode, and R/W/X okay
    asm volatile ("li x6, 0x7f":::"x6");
    asm volatile ("csrw pmpcfg0, x6":::);
}

void switch_m2u_mode(){
    clear_csr (mstatus,MSTATUS_MPP);
    //printf("\nIn the m2u function, the mstatus is 0x%x\n", read_csr(mstatus));
    //printf("\nIn the m2u function, the mepc is 0x%x\n", read_csr(mepc));
    asm volatile ("la x6, 1f    ":::"x6");
    asm volatile ("csrw mepc, x6":::);
    asm volatile ("mret":::);
    asm volatile ("1:":::);
} 

uint32_t mtime_lo(void)
{
  return read_csr(0xBDA);
}


uint32_t get_timer_value()
{
    return mtime_lo();
}

uint32_t get_timer_freq()
{
  return TIMER_FREQ;
}

uint32_t get_instret_value()
{
    return read_csr(minstret);;
}

uint32_t get_cycle_value()
{
    return read_csr(mcycle);
}

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n)
{
  uint32_t start_mtime, delta_mtime;
  uint32_t mtime_freq = get_timer_freq();

  // Don't start measuruing until we see an mtime tick
  uint32_t tmp = mtime_lo();
  do {
    start_mtime = mtime_lo();
  } while (start_mtime == tmp);

  uint32_t start_mcycle = read_csr(mcycle);

  do {
    delta_mtime = mtime_lo() - start_mtime;
  } while (delta_mtime < n);

  uint32_t delta_mcycle = read_csr(mcycle) - start_mcycle;

  return (delta_mcycle / delta_mtime) * mtime_freq
         + ((delta_mcycle % delta_mtime) * mtime_freq) / delta_mtime;
}

uint32_t get_cpu_freq()
{
  uint32_t cpu_freq;

  // warm up
  measure_cpu_freq(1);
  // measure for real
  cpu_freq = measure_cpu_freq(100);

  return cpu_freq;
}

/*
#define CSR_IRQCIP  0xBD0
#define CSR_IRQCIE  0xBD1
#define CSR_IRQCLVL  0xBD2
#define CSR_CEDGE   0xBD3
#define CSR_IRQCINFO  0xBD4
#define CSR_MSIP     0xBD8
#define CSR_MTIMECMP  0xBD9
#define CSR_MTIME   0xBDA
#define CSR_MSTOP   0xBDB
*/
  
void irqc_init ( uint32_t num_irq )
{
  write_csr(0xBD0,0x0);
  write_csr(0xBD1,0x0);
  write_csr(0xBD2,0x0);
  write_csr(0xBD3,0x0);
}


void irqc_enable_interrupt (uint32_t source) {
  set_csr(0xBD1, 1<<source);
}

void irqc_disable_interrupt (uint32_t source){
  clear_csr(0xBD1, 1<<source);
}

void irqc_set_pending(uint32_t source){
  set_csr(0xBD0, 1<<source);
}

void irqc_clear_pending(uint32_t source){
  clear_csr(0xBD0, 1<<source);
}

void irqc_set_level_trig(uint32_t source) {
  set_csr(0xBD2, 1<<source);
}

void irqc_set_posedge_trig(uint32_t source) {
  clear_csr(0xBD2, 1<<source);
  clear_csr(0xBD3,  1<<source);
}

void irqc_set_negedge_trig(uint32_t source) {
  clear_csr(0xBD2, 1<<source);
  set_csr(0xBD3,   1<<source);
}

/*
#define CSR_IRQCIP  0xBD0
#define CSR_IRQCIE  0xBD1
#define CSR_IRQCLVL  0xBD2
#define CSR_CEDGE   0xBD3
#define CSR_IRQCINFO  0xBD4
#define CSR_MSIP     0xBD8
#define CSR_MTIMECMP  0xBD9
#define CSR_MTIME   0xBDA
#define CSR_MSTOP   0xBDB

*/

void wfe() {
  core_wfe();
}

