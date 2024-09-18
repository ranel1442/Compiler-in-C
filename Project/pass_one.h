#ifndef PASS_ONE_H
#define PASS_ONE_H

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

extern int directive_line_result;
extern int instruction_line_result;
extern int IC_copy;
void the_first_pass(FILE *file);

#endif /* PASS_ONE_H */

