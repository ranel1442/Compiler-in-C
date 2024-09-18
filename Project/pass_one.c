#include <stdio.h>
#include <string.h>
#include "pre_first_pass.h"
#include "utilities.h"
#include "memory.h"
#include "instruction_line.h"
#define MAX_LINE_LENGTH 80

int directive_line_result = 0;  /* global variable to save directive_line_result result */
int instruction_line_result = 0; /* global variable to save instruction_line_result result */
int IC_copy=0;/*We will save a copy of ic because we will reset it during the second pass*/


/*The function that makes the first pass checks the type of the sentence using check_line empty sentences of type 1 and 2 it ignores and continues and sentences of type 3 and 4 it processes using check_directive_line and instruction_line and finally unites the two types of memory using memory_connection*/
void the_first_pass(FILE *file) {

    char line[MAX_LINE_LENGTH];
    int line_type;

    while (fgets(line, sizeof(line), file)) {
	if (file == NULL) {
	    perror("Error opening file");
	}
        line_type = check_line(line);

        /**Checking if the line should be skipped (empty or response) **/
        if (line_type == 1 || line_type == 2) {
            continue; /** Go to the next line in the file **/
        }

        if (line_type == 3) {

            /** Handling directive lines **/
            directive_line_result = check_directive_line(line);
	
        }

        if (line_type == 4) {


            /**  Handling Instruction lines **/
            instruction_line_result = instruction_line(line);
        }

        /** A call to the memory connection function to connect the memory **/
	memory_connection();
	IC_copy=IC;
    }
}

