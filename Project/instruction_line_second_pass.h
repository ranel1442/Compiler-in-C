#ifndef EXTERNAL_LABEL_H
#define EXTERNAL_LABEL_H

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
 #define MAX_LINE_LENGTH1 80
/* ExternalLabel structure definition */
typedef struct ExternalLabel {
    char label[31];
    int *addresses;
    int address_count;
    struct ExternalLabel *next;
} ExternalLabel;

/* Define EntryLabel structure */
typedef struct EntryLabel {
    char label[31];
    int *addresses;
    int address_count;
    struct EntryLabel *next;
} EntryLabel;

/* declaration of global variable head_entry */
extern EntryLabel *head_entry;
/* declaration of head_external global variable */
extern ExternalLabel *head_external;

void find_label(const char *token);

void addExternalLabel(const char *label, int address);

void external_file(char outputFileName[256]);

void addEntryLabel(const char *label, int address);

void entry_file(char outputFileName[256]);



int instruction_line_paas_two(const char *line);


















#endif 

