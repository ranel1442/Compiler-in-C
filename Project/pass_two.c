

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preProsesor.h"
#include "memory.h"
#include "instruction_line_second_pass.h"
#include "pre_first_pass.h"
#include "utilities.h"
#include "instruction_line.h"
#include "pass_one.h"
#include "pass_two.h"




/** The material function for the second pass over the file receives the file and its name, processes it, processes it line by line and with the help of the function instruction_line_paas_two arranges all our memory in a complete and correct way and then creates the ob file as it should */
void the_second_pass(FILE *inputFile, char outputFileName[256]) {
    char line[MAX_LINE_LENGTH];  
    int check_result;            
    char outputObFileName[256];  
    char *dot;                   
    FILE *outputFile;           
    int start_address;        
    int end_address;           
    int i, j;              
    unsigned int value;     

 

    while (fgets(line, sizeof(line), inputFile)) {

        check_result = check_line(line);
        if (check_result != 4) {
            continue; /* Move to the next line because only instruction sentences are important in the second pass */
        }
        instruction_line_paas_two(line); /* Processing a line corresponding to the instruction */
    }

/* Create the name of the output file with the extension .ob */
    strcpy(outputObFileName, outputFileName);
    dot = strrchr(outputObFileName, '.');
    if (dot != NULL) {
        *dot = '\0'; /* remove the existing extension */
    }
    strcat(outputObFileName, ".ob"); /* add extension .ob */

 /* opening the output file for writing */
    outputFile = fopen(outputObFileName, "w");
    if (outputFile == NULL) {
        perror("Failed to create output file");
        return;
    }

/* writing the first line with IC_copy and DC */
    fprintf(outputFile, "%d %d\n", IC_copy-100, DC);

/* write the contents of the memory to the output file */
    start_address = 1500;
    end_address = ((IC_copy + DC ) * 15) - 1;

    for (i = start_address; i <= end_address; i += 15) {
        value = 0;

/* generate the 15-bit binary number */
        for (j = 0; j < 15; j++) {
            value = (value << 1) | (memory[i + j] - '0');
        }

/* writing to the file as an octal number */
        fprintf(outputFile, "%04d %05o\n", i / 15, value);
    }

/* close the output file */
    fclose(outputFile);
}

