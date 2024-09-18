#ifndef INSTRUCTION_LINE_H
#define INSTRUCTION_LINE_H

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

void to_binary(int value, char binari_memory[], int start_index, int length);

int amount_words_and_value(int addressing_method1, int addressing_method2, int address1_value1, int address1_value2, int num_operands);

int instruction_line(const char *line);

#endif /* INSTRUCTION_LINE_H */

