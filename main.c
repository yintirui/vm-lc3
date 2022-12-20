#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#include "common.h"
#include "ops.h"

int g_running = 1;

int signal_handler(int code){
    // singal( )
    g_running = 0;
}

#define PC_START 0x3000

uint16_t memory[1<<16];
uint16_t reg[R_COUNT];

void
read_code(const char* filepath){
    FILE* image = NULL;
    uint16_t origin = 0;
    int len = 0;
    int pc = 0;


    image = fopen(filepath, "rb");
    if (!image) {
        printf("cannot open %s", filepath);
        exit(1);
    }
    
    pc = PC_START;
    do {
        len = fread(&origin, sizeof(origin), 1, image);
        memory[pc++] = origin;
    }while (len > 0);

    printf("there is %d pieces instructions", pc - PC_START);
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

    /* set the PC to starting position */
    /* 0x3000 is the default */
    reg[R_PC] = PC_START;

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
                do_not_handler(instr);
                break;
            // case OP_BR:
            //     do_br_handler(instr);
            //     break;
            // case OP_JMP:
            //     printf("it is %s", "OP_JMP");
            //     break;
            // case OP_JSR:
            //     @{JSR}
            //     break;
            // case OP_LD:
            //     @{LD}
            //     break;
            // case OP_LDI:
            //     @{LDI}
            //     break;
            // case OP_LDR:
            //     @{LDR}
            //     break;
            // case OP_LEA:
            //     @{LEA}
            //     break;
            // case OP_ST:
            //     @{ST}
            //     break;
            // case OP_STI:
            //     @{STI}
            //     break;
            // case OP_STR:
            //     @{STR}
            //     break;
            // case OP_TRAP:
            //     @{TRAP}
            //     break;
            // case OP_RES:
            // case OP_RTI:
            default:
                // @{BAD OPCODE}
                break;
        }
    }
    // @{Shutdown}
}
