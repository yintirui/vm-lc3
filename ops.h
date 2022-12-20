#ifndef _OPS_H
#define _OPS_H

#include <stdint-gcc.h>

#include "common.h"

extern uint16_t reg[R_COUNT];

void op_add_handler(uint16_t instr);

void op_and_handler(uint16_t instr);

void do_not_handler(uint16_t instr);

// 

#endif