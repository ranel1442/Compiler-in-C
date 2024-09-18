#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <string.h>


char *readFile(const char *filename);
void replaceExtension(char *filename, const char *new_extension);


extern const char *fixed_name[];

int true_number(const char *str);

int true_string(char *str);

#endif /* UTILITIES_H */

