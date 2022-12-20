#include "ops.h"

extern uint16_t reg[R_COUNT];

static uint16_t
sign_extend(uint16_t instr, uint8_t width) {
    if (!(instr & 0x1)) {
        instr &= ~((1 << width) -1);
    }
    return instr;
}

/**
 * for updating condition register
*/
void 
update_flags(uint16_t r){
    if (r == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (r & 0x8000) {
        reg[R_COND] = FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
    }
}

void
op_add_handler(uint16_t instr) {
    /* destination register (DR) */
    uint16_t r0 = (instr >> 9) & 0x7;
    /* first operand (SR1) */
    uint16_t r1 = (instr >> 6) & 0x7;
    /* whether we are in immediate mode */
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] + imm5;
    } else {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] + reg[r2];
    }

    update_flags(r0);
}

void
op_and_handler(uint16_t instr) {
    /* destination register (DR) */
    uint16_t r0 = (instr >> 9) & 0x7;
    /* first operand (SR1) */
    uint16_t r1 = (instr >> 6) & 0x7;
    /* whether we are in immediate mode */
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag) {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    } else {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }

    update_flags(r0);
}

void
op_not_handler(uint16_t instr) {
    /* destination register (DR) */
    uint16_t r0 = (instr >> 9) & 0x7;
    /* first operand (SR1) */
    uint16_t r1 = (instr >> 6) & 0x7;
    
    reg[r0] = ~reg[r1];

    update_flags(r0);
}