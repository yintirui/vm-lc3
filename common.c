#include "common.h"

void update_flags(uint16_t r) {
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
    } else if (reg[r] >> 15) {/* a 1 in the left-most bit indicates negative */
    
        reg[R_COND] = FL_NEG;
    } else{
        reg[R_COND] = FL_POS;
    }
}