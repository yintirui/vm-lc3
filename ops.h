#ifndef _OPS_H
#define _OPS_H

#include "common.h"

extern uint16_t reg[R_COUNT];

void op_add_handler(uint16_t instr);

void op_and_handler(uint16_t instr);

void op_not_handler(uint16_t instr);

void op_br_handler(uint16_t instr);

void op_jmp_handler(uint16_t instr);

void op_jsr_handler(uint16_t instr);

void op_ld_handler(uint16_t instr);

void op_ldi_handler(uint16_t instr);

void op_ldr_handler(uint16_t instr);

void op_lea_handler(uint16_t instr);

void op_ret_handler(uint16_t instr);

void op_st_handler(uint16_t instr);

void op_sti_handler(uint16_t instr);

void op_str_handler(uint16_t instr);

void op_trap_handler(uint16_t instr);

#endif