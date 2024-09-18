/*A file intended for behind-the-scenes operations and to support other files and functions*/

#include "utilities.h"
#include <stdio.h>

/* A function to read the contents of a file into a string */
char* readFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = malloc(length + 1); 
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length] = '\0';
    }

    fclose(file);
    return buffer;
}

/* Function to change the extension of a file name */
void replaceExtension(char* filename, const char* new_extension) {
    char* dot = strrchr(filename, '.');
    if (dot) {
        strcpy(dot + 1, new_extension);
    }
}

/* A function to check the correctness of the number The function receives a word that is actually a number checks if it is a correct number and turns it into an integer */
int true_number(const char* str) {
    int sign = 1;
    int i = 0;
    int num = 0; 
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 2048;
        }
        num = num * 10 + (str[i] - '0');
    }

    int result = sign * num; 
    if (result <= -2048 || result > 2047) {
        return 2048;
    }

    return result;
}
/*A function to check the correctness of the number The function receives a word that is actually a number and checks if it is a number that fits the data memory (a number in the data memory can be larger because it has more bits to catch because it does not have an are) and turns it into an integer*/
int true_number_for_data(const char* str) {
    int sign = 1;
    int i = 0;
    int num = 0; 

    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 16384;
        }
        num = num * 10 + (str[i] - '0');
    }

    int result = sign * num; 
    if (result <= -16384 || result > 16383) {
        return 16384;
    }

    return result;
}






/* A function to check the integrity of the string The function accepts a word that is actually a character string, checks if it is a valid string and returns 1 if so */
int true_string(char* str) {
    int len = strlen(str);
    /*Checking the length of the string and the quotation marks at the beginning and end*/ 
    if (len <= 2 || str[0] != '"' || str[len-1] != '"') {

        return 0;
    }

    /* Checking internal characters - only alphanumeric or spaces are allowed*/
    int i;
    for (i = 1; i < len - 2; i++) {
        if (!isalnum(str[i]) && !isspace(str[i])) {


            return 0;
        }
    }

    /*Removing the quotes from the string if it is valid*/ 
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';

    return 1;
}

/*A global array that stores all the reserved words of the assembly */
const char* fixed_name[] = {
    "move", "cmp", "add", "sub", "lea", "clr", "not", "inc",
    "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

