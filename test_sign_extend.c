#include <stdio.h>

#include "ops.h"

static uint16_t
sign_extend1(uint16_t val, int width) {
    if (val >> (width - 1) & 0x1) {
        val |= ~((1 << width) - 1);
    }
    return val;
}

uint16_t sign_extend2(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

int main() {
    uint16_t instr = 0xFFFF & 0x1FF;
    printf("%x\n", instr);
    instr = sign_extend1(instr, 9);
    printf("%d\n", instr);
}