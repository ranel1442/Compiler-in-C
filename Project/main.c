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

int main(int argc, char *argv[]) {
    int i; 
    char asExtension[] = ".as"; /* the suffix we want to add */

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file1> <input_file2> ...\n", argv[0]);
        return 1;
    }
/*Go over the inputs one after the other*/
    for (i = 1; i < argc; i++) {
        char outputFileName[256];
        char inputFileName[256]; /* New variable to store the name of the file with the extension .as */

        FILE *outputFile;

        if (argv[i] == NULL) {
            fprintf(stderr, "Error: argument %d is NULL\n", i);
            continue;
        }

/* adding the extension .as to the file name */
        snprintf(inputFileName, sizeof(inputFileName), "%s%s", argv[i], asExtension);

/* handle the name of the output file */
        strcpy(outputFileName, inputFileName);
        replaceExtension(outputFileName, "am");

/* process the file */
        processFile(inputFileName, outputFileName);

        outputFile = fopen(outputFileName, "r");

        if (outputFile == NULL) {
            perror("Error opening output file");
            continue;
        }
/*The first pass over the file*/
        the_first_pass(outputFile);
/*Reset IC before the second pass*/
        IC = 100;
        rewind(outputFile);

/*A second pass over the file and calling the functions to create the ext and ent file*/
        the_second_pass(outputFile, outputFileName);
        external_file(outputFileName);
        entry_file(outputFileName);

	

        fclose(outputFile);
    }

    return 0;
}

