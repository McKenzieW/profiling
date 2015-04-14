/*
 *      HOMEWORK 6: Universal Machine
 *      Sam Gates (sgates01) and McKenzie Welter (mwelte01)
 *      April 6, 2015
 *      um_emulator.c 
 *      
 *      SUMMARY: main program for Universal Machine emulator. Reads
 *      instructions from input and emulates operations using segmented
 *      memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "um_operations.h"
#include "segmem.h"
#include "uarray.h"

UArray_T read_input(FILE *input);
void emulate_um(Segmem segments, uint32_t *regs);
uint32_t *initialize_registers();

int main(int argc, char **argv) 
{
        FILE *input = NULL;
        if (argc == 1) {
                input = stdin;
        } else {
                for (int i = 1; i < argc; i++) {
                        input = fopen(argv[i], "r");
                        if (input == NULL) {
                                fprintf(stderr, 
                                        "%s: %s %s %s\n",
                                        argv[0], "Could not open file ",
                                        argv[i], "for reading");
                                exit(1);
                        }
                }
        }

        UArray_T seg0 = read_input(input);
        Segmem segments = new_segmem(seg0);
        
        uint32_t *regs = initialize_registers();
        
        emulate_um(segments, regs);

        free_segmem(segments);
        free(regs);
        
        fclose(input);
        return 0;
}

/* allocates memory for 8 registers and initializes to zero */
uint32_t *initialize_registers()
{
        uint32_t *registers = malloc(8 * (sizeof(uint32_t)));
        assert(registers);
        int i;
        for (i = 0; i < 8; i++) {
                registers[i] = 0;
        }
        return registers;
}

/* reads instructions from input and puts them in a segment */
UArray_T read_input(FILE *input)
{       
        /* finds length of file in uint32_t's */
        fseek(input, 0, SEEK_END);
        int file_len = ftell(input) / 4;
        fseek(input, 0, SEEK_SET);
        
        UArray_T seg0 = UArray_new(file_len, sizeof(uint32_t));

        int i, j;
        uint32_t word;
        char c = 0;
        /* loops through all uint32_t's in file */
        for (j = 0; j < file_len; j++) {
                word = 0;
                /* packs input into uint32_t's from big endian order */
                for (i = 3; i >= 0; i--) {
                        c = fgetc(input);
                        word = Bitpack_news(word, 8, 8 * i, c);
                }
                uint32_t *loc;
                loc = UArray_at(seg0, j);
                *loc = word;
        }
        return seg0;
}

/* retrieves and performs instructions */
void emulate_um(Segmem segments, uint32_t *regs)
{
        /* loops through instructions until halt is called */
        while (regs != NULL)
        {
                Um_instruction inst = load(segments, 0, segments->prog_count);
                segments->prog_count++;
                regs = read_instruction(inst, regs, segments);
        }
}



