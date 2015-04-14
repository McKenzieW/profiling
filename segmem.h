/*
 *      HOMEWORK 6: Universal Machine
 *      Sam Gates (sgates01) and McKenzie Welter (mwelte01)
 *      March 29, 2015
 *      segmem.h 
 *      
 *      SUMMARY: This interface creates and manipulates segmented memory
 *               for the universal machine. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "uarray.h"
#include "stack.h"
#include "assert.h"

#ifndef SEGMEM_INCLUDED
#define SEGMEM_INCLUDED

typedef struct Segmem {
        UArray_T segments;
        Stack_T unmapped_segs;
        int mapped_segs;
        int prog_count;
} *Segmem;

Segmem      new_segmem  (UArray_T seg0);
extern void free_segmem (Segmem segments);
uint32_t    map         (Segmem segments, unsigned size);
extern void unmap       (Segmem segments, uint32_t segid);
uint32_t    load        (Segmem segments, uint32_t segid, unsigned offset);
extern void store       (Segmem segments, uint32_t val, uint32_t segid, 
                         unsigned offset);
extern void load_prog   (Segmem segments, uint32_t segid, uint32_t offset);

#endif
