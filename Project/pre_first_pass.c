/*A file whose purpose is to prepare the auxiliary functions required for the first pass, such as defining a place for mapping labels, the type of line you are working on and processing that line (for a line of type 3 instruction, we will write a dedicated file separately because it is very busy to write it here as well)*/
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

#define MAX_LINE_LENGTH 80

LabelEntry *head_label = NULL; /*Global head_label of the linked list*/ 
char binari_memory[15] = "000000000000000"; /*A character array is initialized to 15 zeros*/ 

/*A function that receives a string and an integer and checks if the string is a valid label and the number it receives is actually a flag that prints the error only if the flag is zero (sometimes when checking an operand all types of operands are checked and if it is valid as another operand there is no need to print an error)*/
int is_label(const char *str, int print_errors) {
    int len = strlen(str);
    int i = 0;
    int j = 0;

    if (len >= 1 && isalpha(str[0])) {

        for (i = 1; i < len; i++) {
            if (!isalnum(str[i-1])) {
                if (print_errors) {
                    printf("Error: Label contains invalid characters.\n");
                }
                return 0;
            }
        }

        for (j = 0; j < 24; j++) {  
/* Check that the label is not one of the fixed names that must not be used */
            if (strcmp(str, fixed_name[j]) == 0) {
                if (print_errors) {
                    printf("Error: Label name cannot be a reserved word.\n");
                }
                return 0;
            }
        }

        return 1;
    }

    if (print_errors) {
        printf("Error: Invalid label. %s ::\n",str);
    }
    return 0;
}









/*Function to create a new label node*/ 
LabelEntry* createNode(const char *label, int address, const char *attribute) {
    LabelEntry *newNode = (LabelEntry*)malloc(sizeof(LabelEntry));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }
    strncpy(newNode->label, label, sizeof(newNode->label) - 1);
    newNode->label[sizeof(newNode->label) - 1] = '\0';
    newNode->address = address;
    strncpy(newNode->attribute, attribute, sizeof(newNode->attribute) - 1);
    newNode->attribute[sizeof(newNode->attribute) - 1] = '\0';
    newNode->next = NULL;

    return newNode;
}


/* A function that receives an address name and an attribute to add a node to the label list while being used in the createNode function and in addition it checks if the label already exists and changes its attribute if necessary */
void appendNode(const char *label, int address, const char *attribute) {
    LabelEntry *current = head_label;
    LabelEntry *newNode = createNode(label, address, attribute);
    int i=1;
    /* Check if a node with the same label already exists */
    while (current != NULL) {

        if (strcmp(current->label, label) == 0) {

	i=0;
            /* Node with the same label found */
            if (current->address == 0) {
                current->address = address;  /* Update the address if it was 0 */
            }

            /* Check attribute conflict */
            if ((strcmp(current->attribute, attribute) == 0) ||
                (strcmp(current->attribute, "extern") == 0 && strcmp(attribute, "entry") == 0) ||
                (strcmp(current->attribute, "entry") == 0 && strcmp(attribute, "extern") == 0)) {
                printf("Error: Cannot declare the same label twice with conflicting attributes.\n");
                return;  /* Return without adding a new node */
            }

            /* Replace "code" attribute with the new attribute if applicable */
            if (strcmp(current->attribute, "code") == 0) {
                strncpy(current->attribute, attribute, sizeof(current->attribute) - 1);
                current->attribute[sizeof(current->attribute) - 1] = '\0';
            }

            return;  /* Return without adding a new node since it was updated */
        }
        current = current->next;
    }

    /* If no node with the same label was found, create a new node */
 
	if(i){
    if (head_label == NULL) {
        head_label = newNode;
    } else {
        current = head_label;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

	}
}

/*Function to free the linked list*/ 
void freeList(void) {
    LabelEntry *current = head_label;
    while (current != NULL) {
        LabelEntry *next = current->next;
        free(current);
        current = next;
    }
    head_label = NULL; /*Reset head_label after freeing the list*/ 
}

/*A function to print the list of labels (the function is not used but it is important to have it for debugging purposes)*/
void print_label_list() {
    LabelEntry *current = head_label;  /*A pointer to the current record*/ 

    /*If the list is empty*/ 
    if (current == NULL) {
        printf("The label list is empty.\n");
        return;
    }

    /* Go through a list and print each record Go through a list and print each record*/ 
    while (current != NULL) {
        printf("Label: %s, Address: %d, Attribute: %s\n", 
               current->label, current->address, current->attribute);
        current = current->next;  /*מעבר לרשומה הבאה*/ 
    }
}

/*A function that takes an integer and turns it into a 15-bit binary and puts it into a memory array binari_memory*/
void intToBinary15(int i) {
	int j;
    for ( j = 0; j < 15; j++) {
        /*Shift the integer i to the right by (14 - j) positions and get the least significant bit*/ 
        binari_memory[j] = ((i >> (14 - j)) & 1) ? '1' : '0';
    }
}
/*A function that ran on a string received in the data decomposes it into characters and inserts each character into memory (inserting an array of zeros of '0' reads outside the function*/
void processWord(const char *word) {
	
    int ascii_value;
    int i;
    int j;
    for ( i = 0; word[i] != '\0'; i++) {   
        if (word[i] == '"') {
            continue;  /*If the character is ", skip to the next character*/ 
        }

        ascii_value = (int)word[i];  /*Saving the ASCII value of the character in a variable of type int*/ 
        intToBinary15(ascii_value);  /*Conversion to binary using the 2's complement method*/

	setMemory_data(DC,binari_memory);/*Entering the character into the data memory*/
	DC++;

	binari_memory[15] = '\0';

        }
	

    }

/*The basic sifting function for what type of our line regarding a sentence of type 3 and type 4 does not check if it is correct, it only gives information about the potential of the sentence to be (sentence of type 1 to 4)*/
int check_line(const char *line) {
    int len;
    char *line_copy;
    char *token;
    
    line_copy = (char *)malloc(strlen(line) + 1);
    if (!line_copy) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    strcpy(line_copy, line);

    len = strlen(line_copy);

    /*  Check for empty lines and comments */
    if (len == 0 || line_copy[0] == '\n') {
        free(line_copy);
        return EMPTY_LINE;
    }
	/*Comment sentence check*/
    if (line_copy[0] == ';') {
        free(line_copy);
        return COMMENT_LINE;
    }

    /* Search instructions and commands */
    token = strtok(line_copy, " \t");
	if (token != NULL && (token[0] == '.')){
		free(line_copy);
        	return DIRECTIVE_LINE;
	} else if(strchr(token, ':') != NULL){
			token = strtok(NULL, " \t");
		if (token != NULL && (token[0] == '.')){
        		free(line_copy);
			return DIRECTIVE_LINE;
			}
		}
    free(line_copy);
/*If it is not a sentence of type 1 to 3 we will return 4 without further checking*/
    return INSTRUCTION_LINE;
}

/*A function that receives a sentence after it has been classified as a potential directive, the function makes a final check to see if the sentence is correct, and if so, it classifies it according to the type of directive and enters the translation of the sentence into the data memory and label array*/
int check_directive_line(const char *line) {
    int i = 0, j = 0;
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    char *token_copy2[31];
    char *token_copy[31];
    int len;
    char data_line[MAX_LINE_LENGTH];
    char *string_token = NULL;
    char *extern_token = NULL;
    char *entry_token = NULL;
    int result;
    char processed_line[MAX_LINE_LENGTH * 3]; 





    /*To take care of the fact that a comma can be the end of a word and also a word of its own, we will run on the line and every time there is a comma we will add a space before and after it so that it is sure to be a word of its own*/ 

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

/*token will hold the value of the word we are working on at each step*/
	     token = strtok(line_copy, " \t");


    if (token == NULL) {
        return 0;
    }

    len = strlen(token);

    /* We will check if the first word is a label statement */
    if (token[len - 1] == ':') {
	strncpy(token_copy2, token, MAX_LINE_LENGTH - 1);
        token_copy2[len - 1] = '\0'; /* If so, we will remove the colons at the end */
        if (!is_label(token_copy2, 1)) {
	   printf("only label can finish in ':' and %s not label \n",token_copy2);
            return 0;
        }

	strncpy(token_copy, token, MAX_LINE_LENGTH - 1);
	appendNode(token_copy,DC*(-1),token);
	/*We will insert the label into the list of labels*/ 
        token = strtok(NULL, " \t"); /* Skip to the next word */

    }

    if (token == NULL) {
	printf("need more word in correct line : %d\n",DC+IC);
        return 0;
    }

    /* Checking the instruction type */
    if (strcmp(token, ".data") == 0) {
        token = strtok(NULL, " \t");
        while (token != NULL) {
		if(*token == ','){
			token = strtok(NULL, " \t");		
			if(token == NULL){
				printf("line can not finish in ','\n");
				return 1;
			} 
		}
            if (true_number_for_data(token) == 16384) {

                return 0;
            }
		
                i=true_number_for_data(token);

		intToBinary15(i);/*We created a 15-bit array that we put into the data memory*/
		setMemory_data(DC,binari_memory);/* We entered the number into the data memory*/
		DC++;
		for( i = 0; i < 15; i++) {
    			binari_memory[i] = 0;
		}
		token = strtok(NULL, " \t");

        }

        return 1;

/*We will check that there is only one word after string
 Then we will copy it and insert the copy into the true_string test */ 
 printf("try 2 token is %s",token);
    } else if (strcmp(token, ".string") == 0) {

	token = strtok(NULL, " \t");



        result = true_string(token);
	if(result ==0){
		return 0;/*In case a valid string was not entered*/
	}



	processWord(token);/* We will insert the characters of the word into the data memory*/
	for ( i = 0; i < 15; i++) {
 	   binari_memory[i] = '0';
	}

	setMemory_data(DC,binari_memory);
	DC++;
/*We will insert a line of zeros because the word is over*/


        return  2;

/*Treatment with extern instruction*/
    } else if (strcmp(token, ".entry") == 0) {
        entry_token = (char *)malloc(strlen(token) + 1);
        if (entry_token == NULL) {
            fprintf(stderr, "Memory allocation failed in %d\n",DC+IC);
            return 0;
        }
	token = strtok(NULL, " \t");		
	if(token == NULL) {
		fprintf(stderr, "need label after entry in %d\n",DC+IC);
		return 0;
	}
        strcpy(entry_token, token);
        result = is_label(token, 1);
	if(result==0){
		fprintf(stderr, " label not legal in %d\n",DC+IC);
		return 0;
	} 
	appendNode(entry_token,0,"entry");
        free(entry_token);
        return 3;



/*Treatment with entry instruction*/
    } else if (strcmp(token, ".extern") == 0) {
        extern_token = (char *)malloc(strlen(token) + 1);
        if (extern_token == NULL) {
            fprintf(stderr, "Memory allocation failed in %d\n",DC+IC);
            return 0;
        }
	token = strtok(NULL, " \t");		
	if(token == NULL) {
		fprintf(stderr, "need label after extern in %d\n",DC+IC);
		return 0;
	}
        strcpy(extern_token, token);
        result = is_label(extern_token, 1);
	if(result==0){
		fprintf(stderr, " label not legal in %d\n",DC+IC);
	 return 0;
	}

	appendNode(extern_token,0,"extern");
        free(extern_token);
        return 3;
    }
 





    return 0;
}


/*Checking the address method for address number 0*/
int address0(const char *word) {
    if (word[0] != '#'){

return 2048;
} 


    return true_number(word + 1);
}

/*There is no need to check the addressing method 1 we have the is_label function for that*/
/*Checking the address method for address number 2*/
int address2(const char *word) {
    int i = 0;
    if (word[0] != '*') return 2048;
    word++;
    for (i = 16; i <= 23; i++) {
        if (strcmp(word, fixed_name[i]) == 0) {

            return i - 16;
        }
    }
    return 2048;
}
/*Checking the address method for address number 3*/
int address3(const char *word) {
    int i = 0;
    for (i = 16; i <= 23; i++) {
        if (strcmp(word, fixed_name[i]) == 0) {

            return i - 16;
        }
    }
    return 2048;
}

