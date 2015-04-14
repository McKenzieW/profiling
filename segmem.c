/*
 *      HOMEWORK 6: Universal Machine
 *      Sam Gates (sgates01) and McKenzie Welter (mwelte01)
 *      April 1, 2015
 *      segmem.c 
 *      
 *      SUMMARY: Implementation of the segmem interface. Allocates and
 *      frees space for segmented memory, performs operations that involve
 *      creating and manipulating segments given necessary value stored in
 *      registers, returns information stored in segments when necessary,
 *      and tracks progress of the program.
 */

#include "segmem.h"

const int INIT_CAPACITY = 10;

/* 
 * Produces the initial state of a um's segmented memory. Allocates space
 * for segmented memory, initializes segment zero to hold um instructions,
 * and sets program counter to zero.
 */
Segmem new_segmem (UArray_T seg0)
{
        Segmem new_mem = malloc(sizeof(*new_mem));
        assert(new_mem);

        new_mem->unmapped_segs = Stack_new();
        new_mem->segments = UArray_new(INIT_CAPACITY, sizeof(UArray_T));
        new_mem->mapped_segs = 1;
        new_mem->prog_count = 0;

        /*puts segment 0 in the first index of the uarray storing segmem*/
        UArray_T *ptr;
        ptr = UArray_at(new_mem->segments, 0);
        *ptr = seg0;
        
        return new_mem;
}

/* 
 * frees all memory associated with segmem (stack of unmapped segments,
 * uarrays representing segments, and the overall uarray of segments)
 */
void free_segmem (Segmem segments)
{
        /* change i to 0 if we are losing memory after malloc seg0 */
        int i;
        int size = segments->mapped_segs;
        for (i = 0; i < size; i++) {
                UArray_T *seg;
                seg = UArray_at(segments->segments, i);
                if (*seg != NULL) {
                        UArray_free(seg);
                }
        }
        while (!Stack_empty(segments->unmapped_segs)) {
                uint32_t *index;
                index = (uint32_t *)Stack_pop(segments->unmapped_segs);
                free(index);
        }

        UArray_free(&(segments->segments));
        Stack_free(&(segments->unmapped_segs));
        free(segments);
}

/* creates a new segment in segmem at end of segmem or most recently
 * unmapped index
 */
uint32_t map (Segmem segments, unsigned size)
{
        UArray_T new_seg = UArray_new (size, sizeof(uint32_t));
        unsigned i;

        /* initializes all values in segment to zero */
        for (i = 0; i < size; i++) {
                uint32_t *word;
                word  = UArray_at(new_seg, i);
                *word = 0; 
        }
       
        uint32_t index = segments->mapped_segs;
        int length = UArray_length(segments->segments);

        /* checks for unmapped indices in segmem */
        if (!Stack_empty(segments->unmapped_segs)) {
                uint32_t *temp;
                temp = (uint32_t *)Stack_pop(segments->unmapped_segs);
                index = *temp;
                free(temp);
        } else {
                segments->mapped_segs++;
        }
        if (index >= (uint32_t)length) {
                UArray_resize(segments->segments, length * 2);
        }
        UArray_T *loc;
        loc = UArray_at(segments->segments, index);
        *loc = new_seg;
         
        return index;
}

/* frees memory associated with segment at given index */
void unmap (Segmem segments, uint32_t segid)
{
        UArray_T *seg;
        seg = UArray_at(segments->segments, segid);
        UArray_free(seg);
        *seg = NULL;
        
        /* puts unmapped index on stack */ 
        uint32_t *stack_num = malloc(sizeof(stack_num));
        assert(stack_num);
        *stack_num = segid;
        Stack_push(segments->unmapped_segs, stack_num);
}

/* returns value at index offset in segment at index segid */
uint32_t load (Segmem segments, uint32_t segid, unsigned offset)
{
        UArray_T *seg = NULL;
        seg = UArray_at(segments->segments, segid);

        return *(uint32_t *)UArray_at(*seg, offset);
}

/* stores given value at index offset in segment at index segid */
void store (Segmem segments, uint32_t val, uint32_t segid, 
                   unsigned offset)
{
        UArray_T *seg = NULL;
        seg = UArray_at(segments->segments, segid);
        
        uint32_t *loc;
        loc = UArray_at(*seg, offset);
        *loc = val;       
}

/* duplicates segment at index segid and replaces seg0 and resets prog_count */
void load_prog (Segmem segments, uint32_t segid, uint32_t offset)
{
        UArray_T *seg0;
        seg0 = UArray_at(segments->segments, 0);
        UArray_free(seg0);
        
        UArray_T *seg = NULL;
        seg = UArray_at(segments->segments, segid);
        UArray_T dup = UArray_copy(*seg, UArray_length(*seg));

        *seg0 = dup;
        
        segments->prog_count = offset;
}
