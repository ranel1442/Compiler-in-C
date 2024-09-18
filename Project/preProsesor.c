/*The purpose of the file is actually handling macros, saving the macros in the array, finding them in the location file and changing the source file according to the settings*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "preProsesor.h"

#define MAX_LINE_LENGTH 1024
#define MAX_MACRO_NAME_LENGTH 256

/*A structure for defining a macro*/
typedef struct Macro {
    char name[MAX_MACRO_NAME_LENGTH];
    char *content;
    struct Macro *next;
} Macro;

Macro *head = NULL;

/* A function to copy a string and allocate memory */
char *my_strdup(const char *s) {
    char *d = malloc(strlen(s) + 1); 
    if (d == NULL) return NULL;       
    strcpy(d, s);                     
    return d;
}

/*A function to add a new macro creates a new macro and adds it to the list*/
void addMacro(const char *name, const char *content) {
    Macro *newMacro = (Macro *)malloc(sizeof(Macro));
    strcpy(newMacro->name, name);
    newMacro->content = my_strdup(content); 
    newMacro->next = head;
    head = newMacro;
}

char *findMacro(const char *name) {
    Macro *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->content;
        }
        current = current->next;
    }
    return NULL;
}

/*A function that receives the name of the file with the macros and of the file without the macros, open them, open the file with the macros and copy it to the new file without the macros, the function is used by all the functions we wrote in this file*/
void processFile(const char *inputFilename, const char *outputFilename) {
    FILE *inputFile;
    FILE *outputFile;
    char line[MAX_LINE_LENGTH];
    char macroName[MAX_MACRO_NAME_LENGTH];
    char macroContent[MAX_LINE_LENGTH * 10];
    int inMacro = 0;
    char currentMacroName[MAX_MACRO_NAME_LENGTH];
    char *macroExpansion;
    char *token;
    /** Open the input file for reading **/
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return;
    }
    /** Open the output file for writing **/
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return;
    }
    /** Read the input file line by line **/
    while (fgets(line, sizeof(line), inputFile)) {
        /** Check if the line contains the start or end of a macro **/
        if (sscanf(line, "%s", macroName) == 1) {
            if (strcmp(macroName, "macro") == 0) {
                inMacro = 1;
                sscanf(line + strlen("macro"), "%s", currentMacroName);
                macroContent[0] = '\0';
                continue;
            } else if (strcmp(macroName, "endmacro") == 0) {
                inMacro = 0;
                addMacro(currentMacroName, macroContent);
                continue;
            }
        }
        /** If we are inside a macro, accumulate its content **/

        if (inMacro) {
            strcat(macroContent, line);
        } else {
            /** If we are outside a macro, process the line **/
            token = strtok(line, " \t\n");
            while (token != NULL) {
                macroExpansion = findMacro(token);
                if (macroExpansion) {
                    fputs(macroExpansion, outputFile);
                } else {
                    fputs(token, outputFile);
                    fputs(" ", outputFile);
                }
                token = strtok(NULL, " \t\n");
            }
            fputs("\n", outputFile);
        }
    }
    /** Close the input and output files **/
    fclose(inputFile);
    fclose(outputFile);
}

