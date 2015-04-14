/*
 *      HOMEWORK 6: Universal Machine
 *      Sam Gates (sgates01) and McKenzie Welter (mwelte01)
 *      April 6, 2015
 *      um_operations.c 
 *      
 *      SUMMARY: implementation of um_operations interface. 
 */

#include "um_operations.h"

static inline uint64_t shl(uint64_t word, unsigned bits)
{
        return word << bits;
}

/*
 * shift R logical
 */
static inline uint64_t shr(uint64_t word, unsigned bits)
{
        return word >> bits;
}


uint32_t unpack_instruction(uint32_t inst, unsigned width, unsigned lsb)
{
        unsigned hi = lsb + width; /* one beyond the most significant bit */
        /* different type of right shift */
        return shr(shl(inst, 64 - hi), 64 - width);  
} 

/* unpacks instructions and calls appropriate operations */

uint32_t *read_instruction(Um_instruction inst, uint32_t *regs, 
                           Segmem segments)
{
        unsigned op_width = 4;
        unsigned op_lsb = 28;
        unsigned val_width = 25;
        unsigned reg_width = 3;
        int reg_a;
        uint32_t op_code = unpack_instruction(inst, op_width, op_lsb);
        
        /* special unpacking for load value */
        if (op_code == 13) {
                uint32_t val = unpack_instruction(inst, val_width, 0);
                reg_a = unpack_instruction(inst, reg_width, val_width);
                return load_value(regs, reg_a, val);
        }
        
        /* halt */
        if (op_code == 7) {
                return NULL;
        } 
        
         /* unpack only registers that are needed for operations */

        int reg_c = unpack_instruction(inst, reg_width, 0); 
        
        if (op_code == 9) {
                return unmap_segment(segments, regs, reg_c);  
        }
        if (op_code == 10) {
                return output(regs, reg_c);
        }
        if (op_code == 11) {
                return input(regs, reg_c);
        } 
        
        int reg_b = unpack_instruction(inst, reg_width, reg_width);

        if (op_code == 12) {
                return load_program(segments, regs, reg_b, reg_c);
        }
        if (op_code == 8) {
                return map_segment(segments, regs, reg_b, reg_c);
        }
        
        reg_a = unpack_instruction(inst, reg_width, (reg_width)*2);
        
        if (op_code == 0) {
                return conditional_move(regs, reg_a, reg_b, reg_c);
        } 
        if (op_code == 1) {
                return segmented_load(segments, regs, reg_a, reg_b, reg_c);
        }
        if (op_code == 2) {
                return segmented_store(segments, regs, reg_a, reg_b, reg_c);
        }
        if (op_code == 3) {
                return addition(regs, reg_a, reg_b, reg_c);
        }
        if (op_code == 4) {
                return multiplication(regs, reg_a, reg_b, reg_c);
        }
        if (op_code == 5) {
                return division(regs, reg_a, reg_b, reg_c);
        }
        if (op_code == 6) {
                return bitwise_nand(regs, reg_a, reg_b, reg_c);
        }
        
        /* terminates program is an invalid op_code is provided */
        return NULL;
}

/*******************************************************************************
*  The following functions perform the UM operations indicated by their names  *
******************************************************************************/
uint32_t *conditional_move(uint32_t *regs, int ra, int rb, int rc)
{
        uint32_t reg_c = regs[rc];
        if (reg_c != 0) {
                regs[ra] = regs[rb]; 
        }
        return regs;
}

uint32_t *segmented_load(Segmem segments, uint32_t *regs, int ra, int rb, 
                         int rc)
{
        regs[ra] = load(segments, regs[rb], regs[rc]);
        return regs;
}

uint32_t *segmented_store(Segmem segments, uint32_t *regs, int ra, int rb, 
                          int rc)
{
        store(segments, regs[rc], regs[ra], regs[rb]);
        return regs;
}

uint32_t *addition(uint32_t *regs, int ra, int rb, int rc)
{
        regs[ra] = (regs[rb] + regs[rc]);
        return regs;
}

uint32_t *multiplication(uint32_t *regs, int ra, int rb, int rc)
{
        regs[ra] = (regs[rb] * regs[rc]);
        return regs;
}

uint32_t *division(uint32_t *regs, int ra, int rb, int rc)
{
        regs[ra] = (regs[rb] / regs[rc]);
        return regs;
}

uint32_t *bitwise_nand(uint32_t *regs, int ra, int rb, int rc)
{
        regs[ra] = ~(regs[rb] & regs[rc]);
        return regs;
}

uint32_t *map_segment(Segmem segments, uint32_t *regs, int rb, int rc)
{
        regs[rb] = map(segments, regs[rc]);
        return regs;
}

uint32_t *unmap_segment(Segmem segments, uint32_t *regs, int rc)
{
        unmap(segments, regs[rc]);
        return regs;
}

uint32_t *output(uint32_t *regs, int rc)
{
        fprintf(stdout,"%c", (char)regs[rc]);
        return regs;
}

uint32_t *input(uint32_t *regs, int rc)
{
        uint32_t input;
        input = (uint32_t)fgetc(stdin);
        regs[rc] = input;
        return regs;
}

uint32_t *load_program(Segmem segments, uint32_t *regs, int rb, int rc)
{
        if (regs[rb] == 0) {
                segments->prog_count = regs[rc];
                return regs;
        }
        
        load_prog(segments, regs[rb], regs[rc]);
        return regs;
}

uint32_t *load_value(uint32_t *regs, int ra, uint32_t val)
{
        regs[ra] = val;
        return regs;
}
