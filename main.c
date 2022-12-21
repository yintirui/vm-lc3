#include <stdio.h>
#include <stdlib.h>

#include "ops.h"

int g_running = 1;

int signal_handler(int code){
    // singal( )
    g_running = 0;
}

static uint16_t 
lc3_swap16(uint16_t value) {
    return ((value & 0xFF) << 8) | ((value & 0xFF00) >> 8);
}

void
read_code(const char* filepath){
    FILE* image = NULL;
    uint16_t origin = 0, i = 0;
    int len = 0;

    image = fopen(filepath, "rb");
    if (!image) {
        printf("cannot open %s", filepath);
        exit(1);
    }

    len = fread(&origin, sizeof(uint16_t), 1, image);
    if(len != 1) {
        printf("cannot read first code margin");
        exit(1);
    }
    origin = lc3_swap16(origin);
    reg[R_PC] = origin;

    do {
        len = fread(memory + origin + i, sizeof(uint16_t), 1, image);
        memory[origin + i] = lc3_swap16(memory[origin + i]);
        i++;
    }while (len == 1);

    printf("there is %d pieces instructions\n", i);
}

uint16_t mem_read(uint16_t addr){
    return memory[addr];
}

int 
main(int argc, const char* argv[]) {


    if (argc != 2) {
        printf("%s [image-file1]\n", argv[0]);
        exit(2);
    }
    read_code(argv[1]);

    /* since exactly one condition flag should be set at any given time, set the Z flag */
    reg[R_COND] = FL_ZRO;

    while (g_running)
    {
        /* FETCH */
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;
        
        switch (op)
        {
            case OP_ADD:
                op_add_handler(instr);
                break;
            case OP_AND:
                op_and_handler(instr);
                break;
            case OP_NOT:
                op_not_handler(instr);
                break;
            case OP_BR:
                op_br_handler(instr);
                break;
            case OP_JMP:
                op_jmp_handler(instr);
                break;
            case OP_JSR:
                op_jsr_handler(instr);
                break;
            case OP_LD:
                op_ld_handler(instr);
                break;
            case OP_LDI:
                op_ldi_handler(instr);
                break;
            case OP_LDR:
                op_ldr_handler(instr);
                break;
            case OP_LEA:
                op_lea_handler(instr);
                break;
            case OP_ST:
                op_st_handler(instr);
                break;
            case OP_STI:
                op_sti_handler(instr);
                break;
            case OP_STR:
                op_str_handler(instr);
                break;
            case OP_TRAP:
                op_trap_handler(instr);
                break;
            case OP_RES:
            case OP_RTI:
            default:
                printf("\nbad code occurs!!\n");
                break;
        }
    }
    return 0;
}
