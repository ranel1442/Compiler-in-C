/*The file is designed to handle an instruction-type row and insert it into memory during the first pass*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "preProsesor.h"
#include "memory.h"
#include "instruction_line_second_pass.h"
#include "pre_first_pass.h"
#include "utilities.h"
#include "instruction_line.h"
#include "pass_one.h"
 
/*Global variables */ 

char binari_memory0[15] = "000000000000000"; /*Here the first word that the line produces will be saved*/ 
char binari_memory1[15] = "000000000000000"; /*Here the second word that the line produces will be saved*/ 
char binari_memory2[15] = "000000000000000"; /* This will save the third word that the line produces*/ 

int binari_memory_flag1 = 0;   /* Flag to save if operand 1 is a label*/
int binari_memory_flag2 = 0;   /* Flag to save if operand 2 is a label*/

/**
Converting a number to the 2's complement method and inserting the binary into the function array receives the numerical value that we want to insert in the binary and from which place to which place so that we can insert only after the place of are 
 */
void to_binary(int value, char binari_memory[], int start_index, int length) {
	int i;
    for ( i = 0; i < length; i++) {
        binari_memory[start_index + i] = ((value >> i) & 1) ? '1' : '0';
    }
}

/*A function that checks and calculates the correct values ​​for operands receives the 2 operands and what is the addressing method of each and the amount of operands and updates it in the appropriate memory array*/
int amount_words_and_value(int addressing_method1, int addressing_method2, int address1_value1, int address1_value2, int num_operands) {
    if (num_operands == 1) {
        /* Skip this part if num_operands is equal to 1*/ 
        goto skip_addressing_method1;
    }

    /*Checking the addressing method of the first operand*/ 
    if (addressing_method1 == 0) {
        /*Converting the value to the 2's complement method and inserting it into the array*/ 
        to_binary(address1_value1, binari_memory1, 3, 13);
        binari_memory1[2] = '1';
    } else if (addressing_method1 == 1) {
        binari_memory_flag1 = 1;
    } else if (addressing_method1 == 2 || addressing_method1 == 3) {
        to_binary(address1_value1, binari_memory1, 6, 3);
        binari_memory1[2] = '1';

        /*Checking the addressing method of the second operand*/ 
        if (addressing_method2 == 2 || addressing_method2 == 3) {
            to_binary(address1_value2, binari_memory1, 3, 3);/*60*/
            return 2;
        }
    }

skip_addressing_method1:
    /* Checking the addressing method of the second operand*/ 
    if (addressing_method2 == 0) {
        to_binary(address1_value2, binari_memory2, 3, 13);
        binari_memory2[2] = '1';
    } else if (addressing_method2 == 1) {
        binari_memory_flag2 = 1;
    } else if (addressing_method2 == 2 || addressing_method2 == 3) {
        to_binary(address1_value2, binari_memory2, 3, 3);
        binari_memory2[2] = '1';
    }

    return 1;
}

/*A function that receives a sentence and checks that it is a valid instruction sentence according to the type of instruction according to the number of operands and according to the type of operands*/
/*A small explanation of the function. It checks if there is a statement on a label. If so, it enters it into a list of labels. After that, it checks what the type of instruction is, and according to that, it checks how many operands it has and if they are all valid. When encountering a label, it leaves an empty space in memory.*/
int instruction_line(const char *line) {
    int addressing_method1 = 4; 
    int addressing_method2 = 4;  
    int address1_value1 = 2048;  
    int address1_value2 = 2048;  
	int i=0;
	int j=0;
    char line_copy[MAX_LINE_LENGTH];
	char *token;
	int len;
    char compressed_line[MAX_LINE_LENGTH];
    char processed_line[MAX_LINE_LENGTH * 3];
    int result;
	/*decimal_counter++;*/

/*Check that line size is up to 80 characters*/
    if (strlen(line) > 80) {
        printf("Error: Line exceeds the maximum allowed length of 80 characters. in %d\n",IC);
 
	return 0;
    }

    /*We'll add a space before and after the character ',' in the original line, that way we won't get an operand with a comma attached to it*/ 

	for (j = 0, i = 0; line[i] != '\0'; i++) {
	    if (line[i] == ',') {
		processed_line[j] = ' ';
		j++;

		processed_line[j] = ',';
		j++;

		processed_line[j] = ' ';
		j++;
	    } else {
		processed_line[j] = line[i];
		j++;
	    }
	}

    processed_line[j] = '\0';
    strncpy(line_copy, processed_line, MAX_LINE_LENGTH - 1);

/*token will hold the words we will work on*/
     token = strtok(line_copy, " \t");

    if (token == NULL) {

        return 0;
    }

     len = strlen(token);
    if (token[len - 1] == ':') {
        token[len - 1] = '\0'; /* Removing the colon to check a label*/ 
        if (is_label(token, 1)) {
	    appendNode(token,IC,"code");/*Inserts a new label into the list of labels*/
            token = strtok(NULL, " \t");


            if (token == NULL) {
	printf("need more word in %d\n",IC);
                return 0;  
            }
        } else {
	printf("not a legal label in %d\n",IC);
            return 0; 
        }
    }

    /* We will copy the line and delete all the white characters from it*/


	for (i = 0; line[i] != '\0'; i++) {
	    if (!isspace(line[i])) {
		compressed_line[j] = line[i];
		j++;  
	    }
	}
    compressed_line[j] = '\0';

    /*We will check if the character ',' exists twice in a row*/ 
    if (strstr(compressed_line, ",,") != NULL) {
        printf("Error: Consecutive commas are not allowed in the line. in %d\n",IC);

        return 0;
    }

    line_copy[MAX_LINE_LENGTH - 1] = '\0';

	if (token == NULL) {
	    fprintf(stderr, "Error: token is NULL after strtok in %d\n",IC);
	    return 0;
	}
    /*Checking if the word is one of the instructions*/ 




    if (strcmp(token, "mov") == 0) {
        /* Let's move on to the next word in the line*/
        token = strtok(NULL, " \t");
        if (token == NULL) {
            printf("Error: Missing operands after 'mov'. in %d\n",IC);
            return 0;
        }

        /*Let's check the first operand*/ 
        address1_value1 = address0(token);
        if (address1_value1 != 2048) {
            addressing_method1 = 0;
        } else {
            address1_value1 = address2(token);
            if (address1_value1 != 2048) {
                addressing_method1 = 2;
            } else {
                address1_value1 = address3(token);
                if (address1_value1 != 2048) {
                    addressing_method1 = 3;
                } else {
                    if (is_label(token, 1)) {

                        addressing_method1 = 1;
                    } else {
                        printf("Error: Invalid operand '%s' after 'mov'.\n", token);
                        return 0;
                    }
                }
            }
        }

        /*We will continue to the next word in the line*/ 
        token = strtok(NULL, " \t");
        if (token == NULL || strcmp(token, ",") != 0) {
            printf("Error: A comma is required between operands. in %d\n",IC);
            return 0;
        }

        /**/ 
        token = strtok(NULL, " \t");
        if (token == NULL) {
            printf("Error: Missing second operand after 'mov'. in %d\n",IC);
            return 0;
        }

        /*We will continue to the next word after the comma*/ 
        address1_value2 = address2(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 2;
        } else {
            address1_value2 = address3(token);
            if (address1_value2 != 2048) {
                addressing_method2 = 3;
            } else {
                if (is_label(token, 1)) {

                    addressing_method2 = 1;
                } else {
                    printf("Error: Invalid operand '%s' after 'mov'.\n", token);
                    return 0;
                }
            }
        }

        /*update binari_memory0*/ 
        binari_memory0[2] = '1';
        binari_memory0[addressing_method2 + 3] = '1';
        binari_memory0[addressing_method1 + 7] = '1';

        /*Saving the value in memory*/ 
        setMemory(IC, binari_memory0);
        IC++;
        memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

        /*A call to the amount_words_and_value function */ 
         result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 2);

        if (result == 2) {
            setMemory(IC, binari_memory1);
            IC++;
            memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
            return 1;
        }

        if (binari_memory_flag1 == 1 && binari_memory_flag2 == 1) {
            IC += 2;
            binari_memory_flag1 = 0;
            binari_memory_flag2 = 0;
            return 1;
        }

        if (binari_memory_flag1 == 0 && binari_memory_flag2 == 0) {
            setMemory(IC, binari_memory1);
            setMemory(IC + 1, binari_memory2);
            IC += 2;
            memset(binari_memory1, '0', 15); /* reset binari_memory1*/
            memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
            return 1;
        }

        if (binari_memory_flag1 == 1) {
            IC++;
            setMemory(IC, binari_memory2);
            IC++;
            memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
            binari_memory_flag1 = 0;
            return 1;
        }

        if (binari_memory_flag2 == 1) {
            setMemory(IC, binari_memory1);
            IC++;
            IC++;
            memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
            binari_memory_flag2 = 0;
            return 1;
        }
    } else if (strcmp(token, "cmp") == 0) {
        /*Let's move on to the next word in the line*/ 
        token = strtok(NULL, " \t");
        if (token == NULL) {
            printf("Error: Missing operands after 'cmp'. in %d\n",IC);
            return 0;
        }

        /*Let's check the first operand*/ 
        address1_value1 = address0(token);
        if (address1_value1 != 2048) {
            addressing_method1 = 0;
        } else {
            address1_value1 = address2(token);
            if (address1_value1 != 2048) {
                addressing_method1 = 2;
            } else {
                address1_value1 = address3(token);
                if (address1_value1 != 2048) {
                    addressing_method1 = 3;
                } else {
                    if (is_label(token, 1)) {

                        addressing_method1 = 1;
                    } else {
                        printf("Error: Invalid operand '%s' after 'cmp'.\n", token);
                        return 0;
                    }
                }
            }
        }

        /*We will continue to the next word in the line*/ 
        token = strtok(NULL, " \t");
        if (token == NULL || strcmp(token, ",") != 0) {
            printf("Error: A comma is required between operands. in %d\n",IC);
            return 0;
        }


        /*We will continue to the next word after the comma*/ 
        token = strtok(NULL, " \t");
        if (token == NULL) {
            printf("Error: Missing second operand after 'cmp'. in %d\n",IC);
            return 0;
        }

        /* Let's check the second operand*/
        address1_value2 = address0(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 0;
        } else {
            address1_value2 = address2(token);
            if (address1_value2 != 2048) {
                addressing_method2 = 2;
            } else {
                address1_value2 = address3(token);
                if (address1_value2 != 2048) {
                    addressing_method2 = 3;
                } else {
                    if (is_label(token, 1)) {

                        addressing_method2 = 1;
                    } else {
                        printf("Error: Invalid operand '%s' after 'cmp'.\n", token);
                        return 0;
                    }
                }
            }
        }


        /* reset binari_memory0*/
        binari_memory0[2] = '1';
        binari_memory0[addressing_method2 + 3] = '1';/* cmp*/
        binari_memory0[addressing_method1 + 7] = '1';/* cmp*/
        binari_memory0[11] = '1'; /* cmp*/





        /*Saving the value in memory*/ 
        setMemory(IC, binari_memory0);
        IC++;
        memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

        /*A call to the amount_words_and_value function*/ 
         result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 2);

        if (result == 2) {
            setMemory(IC, binari_memory1);
            IC++;
            memset(binari_memory1, '0', 15); /* reset binari_memory1*/
            return 1;
        }

        if (binari_memory_flag1 == 1 && binari_memory_flag2 == 1) {
            IC += 2;
            binari_memory_flag1 = 0;
            binari_memory_flag2 = 0;
            return 1;
        }

        if (binari_memory_flag1 == 0 && binari_memory_flag2 == 0) {
            setMemory(IC, binari_memory1);
            setMemory(IC + 1, binari_memory2);
            IC += 2;
            memset(binari_memory1, '0', 15); /* reset binari_memory1*/
            memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
            return 1;
        }

        if (binari_memory_flag1 == 1) {

            IC++;
            setMemory(IC, binari_memory2);
            IC++;
            memset(binari_memory2, '0', 15); /* reset binari_memory2*/
            binari_memory_flag1 = 0;
            return 1;
        }

        if (binari_memory_flag2 == 1) {
            setMemory(IC, binari_memory1);
            IC++;
            IC++;
            memset(binari_memory1, '0', 15); /* reset binari_memory1*/
            binari_memory_flag2 = 0;

            return 1;
        }
    }else if (strcmp(token, "add") == 0) {
    /*Let's move on to the next word in the line*/ 
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operands after 'add'. in %d\n",IC);
        return 0;
    }

    /*Let's check the first operand*/ 
    address1_value1 = address0(token);
    if (address1_value1 != 2048) {
        addressing_method1 = 0;
    } else {
        address1_value1 = address2(token);
        if (address1_value1 != 2048) {
            addressing_method1 = 2;
        } else {
            address1_value1 = address3(token);
            if (address1_value1 != 2048) {
                addressing_method1 = 3;
            } else {
                if (is_label(token, 1)) {

                    addressing_method1 = 1;
                } else {
                    printf("Error: Invalid operand '%s' after 'add'.\n", token);
                    return 0;
                }
            }
        }
    }

    /*Let's move on to the next word in the line*/ 
    token = strtok(NULL, " \t");
    if (token == NULL || strcmp(token, ",") != 0) {
        printf("Error: A comma is required between operands. in %d\n",IC);
        return 0;
    }

    /*We will continue to the next word after the comma*/ 
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing second operand after 'add'. in %d\n",IC);
        return 0;
    }

    /*Let's check the second operand*/ 
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'add'.\n", token);
                return 0;
            }
        }
    }

    /* update binari_memory0*/
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';/* add*/ 
    binari_memory0[addressing_method1 + 7] = '1';/* add*/ 
    binari_memory0[12] = '1'; /* add*/ 

    /*Saving the value in memory*/ 
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /* reset binari_memory0*/

    /*A call to the amount_words_and_value function*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 2);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag1 == 1 && binari_memory_flag2 == 1) {

        IC += 2;
        binari_memory_flag1 = 0;
        binari_memory_flag2 = 0;
        return 1;
    }

    if (binari_memory_flag1 == 0 && binari_memory_flag2 == 0) {
        setMemory(IC, binari_memory1);
        setMemory(IC + 1, binari_memory2);
        IC += 2;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
        return 1;
    }

    if (binari_memory_flag1 == 1) {
        IC++;
        setMemory(IC, binari_memory2);
        IC++;
        memset(binari_memory2, '0', 15); /* reset binari_memory2*/
        binari_memory_flag1 = 0;
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        setMemory(IC, binari_memory1);
        IC++;
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        binari_memory_flag2 = 0;
        return 1;
    }
}else if (strcmp(token, "sub") == 0) {
    /*Let's move on to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operands after 'sub'. in %d\n",IC);
        return 0;
    }

    /* Let's check the first operand*/
    address1_value1 = address0(token);
    if (address1_value1 != 2048) {
        addressing_method1 = 0;
    } else {
        address1_value1 = address2(token);
        if (address1_value1 != 2048) {
            addressing_method1 = 2;
        } else {
            address1_value1 = address3(token);
            if (address1_value1 != 2048) {
                addressing_method1 = 3;
            } else {
                if (is_label(token, 1)) {

                    addressing_method1 = 1;
                } else {
                    printf("Error: Invalid operand '%s' after 'sub'.\n", token);
                    return 0;
                }
            }
        }
    }

    /* Let's move on to the next word in the line*/ 
    token = strtok(NULL, " \t");
    if (token == NULL || strcmp(token, ",") != 0) {
        printf("Error: A comma is required between operands. in %d\n",IC);
        return 0;
    }

    /*We will continue to the next word after the comma*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing second operand after 'sub'. in %d\n",IC);
        return 0;
    }

    /* Let's check the second operand*/ 
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'sub'.\n", token);
                return 0;
            }
        }
    }

    /* update binari_memory0*/
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';/* sub*/
    binari_memory0[addressing_method1 + 7] = '1';/* sub*/
    binari_memory0[11] = '1'; /* sub*/ 
    binari_memory0[12] = '1'; /* sub*/ 

    /* Saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /*A call to the amount_words_and_value function*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 2);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag1 == 1 && binari_memory_flag2 == 1) {
        IC += 2;
        binari_memory_flag1 = 0;
        binari_memory_flag2 = 0;
        return 1;
    }

    if (binari_memory_flag1 == 0 && binari_memory_flag2 == 0) {
        setMemory(IC, binari_memory1);
        setMemory(IC + 1, binari_memory2);
        IC += 2;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
        return 1;
    }

    if (binari_memory_flag1 == 1) {
        IC++;
        setMemory(IC, binari_memory2);
        IC++;
        memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
        binari_memory_flag1 = 0;
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        setMemory(IC, binari_memory1);
        IC++;
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        binari_memory_flag2 = 0;
        return 1;
    }
}else if (strcmp(token, "lea") == 0) {
    /*  Let's move on to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operands after 'lea'. in %d\n",IC);
        return 0;
    }

    /* We will check if the word is a valid label*/
    if (is_label(token, 1)) {

        addressing_method1 = 1;
    } else {
        printf("Error: Invalid operand '%s' after 'lea'.\n", token);
        return 0;
    }

    /*Let's move on to the next word in the line */
    token = strtok(NULL, " \t");
    if (token == NULL || strcmp(token, ",") != 0) {
        printf("Error: A comma is required between operands. in %d\n",IC);
        return 0;
    }

    /* We will continue to the next word after the comma*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing second operand after 'lea'. in %d\n",IC);
        return 0;
    }

    /*Let's check the second operand*/ 
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'lea'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';/* lea*/ 
    binari_memory0[addressing_method1 + 7] = '1';/* lea*/ 
    binari_memory0[13] = '1'; /* lea*/ 

    /*  Saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /* A call to the amount_words_and_value function*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 2);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag1 == 1 && binari_memory_flag2 == 1) {
        IC += 2;
        binari_memory_flag1 = 0;
        binari_memory_flag2 = 0;
        return 1;
    }

    if (binari_memory_flag1 == 0 && binari_memory_flag2 == 0) {
        setMemory(IC, binari_memory1);
        setMemory(IC + 1, binari_memory2);
        IC += 2;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        memset(binari_memory2, '0', 15); /* reset binari_memory2*/
        return 1;
    }

    if (binari_memory_flag1 == 1) {
        IC++;
        setMemory(IC, binari_memory2);
        IC++;
        memset(binari_memory2, '0', 15); /* reset binari_memory2*/
        binari_memory_flag1 = 0;
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        setMemory(IC, binari_memory1);
        IC++;
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        binari_memory_flag2 = 0;
        return 1;
    }
}else if (strcmp(token, "clr") == 0) {
    /* Let's move on to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'clr'. in %d\n",IC);
        return 0;
    }

    /*Let's check the first operand*/ 
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'clr'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[11] = '1'; /* clr*/ 
    binari_memory0[13] = '1'; /*  clr*/

    /*Saving the value in memory*/ 
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /* reset binari_memory0*/

/*call to the amount_words_and_value function*/
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /* reset binari_memory2*/
    return 1;
}else if (strcmp(token, "not") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'not'. in %d\n",IC);
        return 0;
    }

/*we will check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'not'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[12] = '1'; /* not*/ 
    binari_memory0[13] = '1'; /*  not*/

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;

    memset(binari_memory0, '0', 15); /* reset binari_memory0*/

/*call to the amount_words_and_value function*/
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
    return 1;
}else if (strcmp(token, "inc") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'inc'. in %d\n",IC);
        return 0;
    }

/*we will check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'inc'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[11] = '1'; /* inc*/ 
    binari_memory0[12] = '1'; /* inc*/ 
    binari_memory0[13] = '1'; /*  inc*/

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /* reset binari_memory0*/ 

/*call to the amount_words_and_value function*/
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
    return 1;
}else if (strcmp(token, "dec") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'dec'. in %d\n",IC);
        return 0;
    }

/* check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'dec'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[14] = '1'; /* dec*/ 

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /* reset binari_memory0*/

    /*function call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
    return 1;
}
else if (strcmp(token, "red") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'red'. in %d\n",IC);
        return 0;
    }

/*we will check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        address1_value2 = address3(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 3;
        } else {
            if (is_label(token, 1)) {

                addressing_method2 = 1;
            } else {
                printf("Error: Invalid operand '%s' after 'red'.\n", token);
                return 0;
            }
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[11] = '1'; /* red*/ 
    binari_memory0[12] = '1'; /* red*/ 
    binari_memory0[14] = '1'; /*  red*/

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /*fynction call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /* reset binari_memory2*/
    return 1;
}else if (strcmp(token, "prn") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'prn'. in %d\n",IC);
        return 0;
    }

/* check the first operand*/
    address1_value2 = address0(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 0;
    } else {
        address1_value2 = address2(token);
        if (address1_value2 != 2048) {
            addressing_method2 = 2;
        } else {
            address1_value2 = address3(token);
            if (address1_value2 != 2048) {
                addressing_method2 = 3;
            } else {
                if (is_label(token, 1)) {

                    addressing_method2 = 1;
                } else {
                    printf("Error: Invalid operand '%s' after 'prn'.\n", token);
                    return 0;
                }
            }
        }
    }

    /* update binari_memory0*/
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[13] = '1'; /*  prn*/
    binari_memory0[14] = '1'; /*  prn*/

/* saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;

    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /*function call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /* reset binari_memory2*/
    return 1;
}else if (strcmp(token, "jmp") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'jmp'. in %d\n",IC);
        return 0;
    }

/*we will check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        if (is_label(token, 1)) {

            addressing_method2 = 1;
        } else {
            printf("Error: Invalid operand '%s' after 'jmp'.\n", token);
            return 0;
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[11] = '1'; /*  jmp*/
    binari_memory0[14] = '1'; /* jmp*/

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /*function call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;
    memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
    return 1;
}else if (strcmp(token, "bne") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'bne'. in %d\n",IC);
        return 0;
    }

/*we will check the first operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        if (is_label(token, 1)) {

            addressing_method2 = 1;
        } else {
            printf("Error: Invalid operand '%s' after 'bne'.\n", token);
            return 0;
        }
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[12] = '1'; /* bne*/ 
    binari_memory0[14] = '1'; /*  bne*/

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /*reset binari_memory0*/ 

    /*function call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;
        memset(binari_memory1, '0', 15); /*reset binari_memory1*/ 
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;

        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;

    memset(binari_memory2, '0', 15); /* reset binari_memory2*/
    return 1;
}else if (strcmp(token, "jsr") == 0) {
/*go to the next word in the line*/
    token = strtok(NULL, " \t");
    if (token == NULL) {
        printf("Error: Missing operand after 'jsr'. in %d\n",IC);
        return 0;
    }

/*check the operand*/
    address1_value2 = address2(token);
    if (address1_value2 != 2048) {
        addressing_method2 = 2;
    } else {
        if (is_label(token, 1)) {

            addressing_method2 = 1;
        } else {
            printf("Error: Invalid operand '%s' after 'jsr'.\n", token);
            return 0;
        }
    }

    /* update binari_memory0*/
    binari_memory0[2] = '1';
    binari_memory0[addressing_method2 + 3] = '1';
    binari_memory0[11] = '1'; /* jsr*/ 
    binari_memory0[13] = '1'; /* jsr*/ 
    binari_memory0[14] = '1'; /* jsr*/ 

/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;
    memset(binari_memory0, '0', 15); /* reset binari_memory0*/

    /*function call amount_words_and_value*/ 
     result = amount_words_and_value(addressing_method1, addressing_method2, address1_value1, address1_value2, 1);

    if (result == 2) {
        setMemory(IC, binari_memory1);
        IC++;

        memset(binari_memory1, '0', 15); /* reset binari_memory1*/
        return 1;
    }

    if (binari_memory_flag2 == 1) {
        IC++;
        binari_memory_flag2 = 0;
        return 1;
    }

    setMemory(IC, binari_memory2);
    IC++;

    memset(binari_memory2, '0', 15); /*reset binari_memory2*/ 
    return 1;
}else if (strcmp(token, "rts") == 0) {

/*check that there is no word after*/
    token = strtok(NULL, " \t\n");
    if (token != NULL) {

        printf("Error: No operands are allowed after 'rts'. in %d\n",IC);
        return 0;
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[12] = '1'; /* rts*/ 
    binari_memory0[13] = '1'; /* rts*/ 
    binari_memory0[14] = '1'; /*  rts*/




/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;

    memset(binari_memory0, '0', 15); /* reset binari_memory0*/


    return 1;
}else if (strcmp(token, "stop") == 0) {


/*check that there is no word after*/
    token = strtok(NULL, " \t\n");
    if (token != NULL) {

        printf("Error: No operands are allowed after 'rts'. in %d\n",IC);
        return 0;
    }

    /*update binari_memory0*/ 
    binari_memory0[2] = '1';
    binari_memory0[11] = '1'; /* stop*/ 
    binari_memory0[12] = '1'; /* stop*/ 
    binari_memory0[13] = '1'; /* stop*/ 
    binari_memory0[14] = '1'; /*  stop*/




/*saving the value in memory*/
    setMemory(IC, binari_memory0);
    IC++;

    memset(binari_memory0, '0', 15); /* reset binari_memory0*/


    return 1;
}
        printf("Error: No invalid line. in %d\n",IC);
    return 0;

}

