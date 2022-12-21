#include "ops.h"

extern int g_running;

static uint16_t
sign_extend(uint16_t val, uint8_t width) {
    if (val >> (width - 1) & 0x1) {
        val |= ~((1 << width) - 1);
    }
    return val;
}


uint16_t
zero_extend(uint16_t val, uint8_t width) {
    val &= ~((1 << width) -1);
    return val;
}

/**
 * for updating condition register
*/
static void 
update_flags(uint16_t r){
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (reg[r] & 0x8000) {
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

void
op_br_handler(uint16_t instr) {
    uint16_t cond = (instr >> 9) & 0x7;
    if (cond & reg[R_COND]) {
        reg[R_PC] += sign_extend(instr & 0x1FF, 9);
    }
}

void
op_jmp_handler(uint16_t instr) {
    uint16_t base_reg = (instr >> 6) & 0x7;
    if (base_reg == 0x7){
        reg[R_PC] = reg[R_R7];
    } else {
        reg[R_PC] = reg[base_reg];
    }
}

void
op_jsr_handler(uint16_t instr) {
    int bit11 = (instr >> 11) & 0x1;
    
    if (bit11) {
        uint16_t offset = sign_extend(instr & 0x7FF, 11);
        reg[R_PC] = offset;
    } else {
        uint16_t br = (instr >> 6) & 0x7;
        reg[R_PC] += reg[br];
    }

}

void 
op_ld_handler(uint16_t instr) {
    uint16_t dr = (instr >> 9) & 0x7;
    uint16_t offset = sign_extend(instr & 0x1FF, 9);
    reg[dr] = memory[R_PC + offset];
    
    update_flags(dr);
}

void 
op_ldi_handler(uint16_t instr) {
    uint16_t dr = (instr >> 9) & 0x7;
    reg[dr] = memory[memory[dr + sign_extend(instr & 0x1FF, 9)]];
    
    update_flags(dr);
}

void 
op_ldr_handler(uint16_t instr) {
    uint16_t dr = (instr >> 9) & 0x7;
    uint16_t br = (instr >> 6) & 0x7;
    reg[dr] = memory[br + sign_extend(instr & 0x3F, 6)];

    update_flags(dr);
}

void 
op_lea_handler(uint16_t instr) {
    uint16_t dr = (instr >> 9) & 0x7;

    reg[dr] = reg[R_PC] + sign_extend(instr &0x1FF, 9);

    update_flags(dr);
}

void 
op_ret_handler(uint16_t instr) {
    op_jmp_handler(instr);
}

void
op_st_handler(uint16_t instr) {
    uint16_t sr = (instr >> 9) & 0x7;
    
    uint16_t offset = sign_extend(instr & 0x1FF, 9);
    memory[R_PC + offset] = reg[sr];
}

void
op_sti_handler(uint16_t instr) {
    uint16_t sr = (instr >> 9) & 0x7;
    memory[memory[sr + sign_extend(instr & 0x1FF, 9)]] = reg[sr];
}

void 
op_str_handler(uint16_t instr) {
    uint16_t sr = (instr >> 9) & 0x7;
    uint16_t br = (instr >> 6) & 0x7;

    memory[reg[br] + sign_extend(instr & 0x3F, 6)] = reg[sr];
}

void
op_trap_handler(uint16_t instr) {
    uint16_t trapvect8 = instr & 0xFF;
    switch (trapvect8) {
    case 0x20:
        reg[0] = fgetc(stdin);
        reg[0] &= 0x00FF;
        fflush(stdin);
        break;
    case 0x21: {
        uint16_t c = reg[0] & 0xFF;
        fputc(c, stdout);
        fflush(stdout);
        break;
    }
    case 0x22: { 
        uint16_t addr = reg[0];
        while(memory[addr] != 0) {
            int c = memory[addr] & 0xFF;
            fputc(c, stdout);
            fflush(stdout);
            addr += 1;
        }
        break;
    }
    case 0x23:
        fputc('>', stdout);
        fflush(stdout);
        reg[0] = fgetc(stdin);
        reg[0] &= 0x00FF;
        fflush(stdin);
        break;
    case 0x24: {
        uint16_t addr = reg[0];
        while(memory[addr] != 0) {
            int low = memory[addr] & 0x00FF;
            if(!low) break;
            fputc(low, stdout);
            int high = memory[addr] & 0xFF00;
            if(!high) break;
            fputc(high, stdout);
            addr += 1;
        }
        fflush(stdout);
        break;
    }
    case 0x25:
        printf("\nHALT\n");
        g_running = 0;
        break;
    default:
        break;
    }
    // reg[R_R7] = reg[R_PC];
    // reg[R_PC] = memory[zero_extend(instr & 0xFF, 8)];
}