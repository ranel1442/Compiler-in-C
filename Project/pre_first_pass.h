#ifndef PRE_FIRST_PASS_H
#define PRE_FIRST_PASS_H
#define MAX_LINE_LENGTH 80

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

#define EMPTY_LINE 1
#define COMMENT_LINE 2
#define DIRECTIVE_LINE 3
#define INSTRUCTION_LINE 4

/*Structure of a label*/
typedef struct {
    char label[31]; 
    int address;
    char attribute[10]; 
    struct LabelEntry *next;

} LabelEntry;

extern LabelEntry *head_label;

void appendNode(const char *label, int address, const char *attribute);
void freeList(void);

int is_label(const char *str, int print_errors);

int add_label(LabelEntry *table, const char *label, int address, const char *attribute, int *size);

void print_label_list(void);

int check_line(const char *line);

int check_directive_line(const char *line);


int address0(const char *word);
int address2(const char *word);
int address3(const char *word);

#endif /* PRE_FIRST_PASS_H */

