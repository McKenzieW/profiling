/*
 *      HOMEWORK 6: Universal Machine
 *      Sam Gates (sgates01) and McKenzie Welter (mwelte01)
 *      March 29, 2015
 *      um_operations.h 
 *      
 *      SUMMARY: Interprets UM intructions and performs appropriate
 *               operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segmem.h"
#include "bitpack.h"

#ifndef UM_OPERATIONS_INCLUDED
#define UM_OPERATIONS_INCLUDED

typedef uint32_t Um_instruction;

/* checks for validity of instruction as well */
uint32_t *read_instruction(Um_instruction inst, uint32_t *regs, 
                           Segmem segments);
uint32_t *conditional_move(uint32_t *regs, int ra, int rb, int rc);
uint32_t *segmented_load  (Segmem segments, uint32_t *regs, int ra, int rb, 
                           int rc);
uint32_t *segmented_store (Segmem segments, uint32_t *regs, int ra, int rb, 
                           int rc);
uint32_t *addition        (uint32_t *regs, int ra, int rb, int rc);
uint32_t *multiplication  (uint32_t *regs, int ra, int rb, int rc);
uint32_t *division        (uint32_t *regs, int ra, int rb, int rc);
uint32_t *bitwise_nand    (uint32_t *regs, int ra, int rb, int rc);
uint32_t *halt            (void);
uint32_t *map_segment     (Segmem segments, uint32_t *regs, int rb, int rc);
uint32_t *unmap_segment   (Segmem segments, uint32_t *regs, int rc);
uint32_t *output          (uint32_t *regs, int rc);
uint32_t *input           (uint32_t *regs, int rc);
uint32_t *load_program    (Segmem segments, uint32_t *regs, int rb, int rc);
uint32_t *load_value      (uint32_t *regs, int ra, uint32_t val);

#endif
