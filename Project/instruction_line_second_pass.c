/*The purpose of our file is basically the entire analysis of a line in the second pass (only instruction lines are actually interesting), that is, finding its insertion label to the right place in the memory we saved for it and inserting the ext and ent files according to those labels*/

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
  /*Declaration of global variables at the top of the list of external and entry*/
struct ExternalLabel *head_external = NULL;
struct EntryLabel *head_entry = NULL;
 char binari_memory5[15] = "000000000000000"; 


/*function to add an external element to the list*/
void addExternalLabel(const char *label, int address) {
    ExternalLabel *current = head_external;

/*search for an existing member with the same label*/
    while (current != NULL) {
        if (strcmp(current->label, label) == 0) {
/*we found an existing member, we will add the new address to it*/
            current->addresses = (int *)realloc(current->addresses, (current->address_count + 1) * sizeof(int));
            if (current->addresses == NULL) {
                perror("Failed to reallocate memory for addresses");
                return;
            }
            current->addresses[current->address_count] = address;
            current->address_count++;
            return;
        }
        current = current->next;
    }

/*If we did not find such a member, we will create a new member*/
    ExternalLabel *newNode = (ExternalLabel *)malloc(sizeof(ExternalLabel));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        return;
    }

/*Initialize the values ​​of the new member*/
    strncpy(newNode->label, label, sizeof(newNode->label) - 1);
    newNode->label[sizeof(newNode->label) - 1] = '\0';
    newNode->addresses = (int *)malloc(sizeof(int));
    if (newNode->addresses == NULL) {
        perror("Failed to allocate memory for addresses");
        free(newNode);
        return;
    }
    newNode->addresses[0] = address;
    newNode->address_count = 1;
    newNode->next = head_external;

/*link the new member to the top of the list*/
    head_external = newNode;
}

/*A function that creates the ext file for us only if necessary, it goes through the list of external and for each instance of a name label writes the name of the label and the position in which it appeared in the assembly code*/
void external_file(char outputFileName[256]) {
    ExternalLabel *current;       
    int hasExternalLabels = 0;   
    char extFileName[256];        
    char *dot;                    
    FILE *file;                  
	int i =0;
/** Check if there are external labels in the list **/
    current = head_external;
    while (current != NULL) {
        if (current->address_count > 0) {
            hasExternalLabels = 1;
            break;
        }
        current = current->next;
    }

/** If there are no external labels, there is no need to create the file **/
    if (!hasExternalLabels) {
        return;
    }

/** Create the new file name with the extension .ext **/
    strcpy(extFileName, outputFileName);
    dot = strrchr(extFileName, '.');
    if (dot != NULL) {
        *dot = '\0'; /** Remove the existing suffix **/
    }
    strcat(extFileName, ".ext");

/** Open the file for writing **/
    file = fopen(extFileName, "w");
    if (file == NULL) {
        perror("Error creating external file");
        return;
    }

/** writing to file **/
    current = head_external;
    while (current != NULL) {
        for ( i = 0; i < current->address_count; i++) {
            fprintf(file, "%s %d\n", current->label, current->addresses[i]);
        }
        current = current->next;
    }

/** closing the file **/
    fclose(file);
}




/*function to add an element to the list*/
void addEntryLabel(const char *label, int address) {
    EntryLabel *current = head_entry;

/*search for an existing member with the same label*/
    while (current != NULL) {
        if (strcmp(current->label, label) == 0) {
/*we found an existing member, we will add the new address to it*/
            current->addresses = (int *)realloc(current->addresses, (current->address_count + 1) * sizeof(int));
            if (current->addresses == NULL) {
                perror("Failed to reallocate memory for addresses");
                return;
            }
            current->addresses[current->address_count] = address;
            current->address_count++;
            return;
        }
        current = current->next;
    }

/*If we did not find such a member, we will create a new member*/
    EntryLabel *newNode = (EntryLabel *)malloc(sizeof(EntryLabel));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        return;
    }

 /*Initialize the values ​​of the new member*/
    strncpy(newNode->label, label, sizeof(newNode->label) - 1);
    newNode->label[sizeof(newNode->label) - 1] = '\0';
    newNode->addresses = (int *)malloc(sizeof(int));
    if (newNode->addresses == NULL) {
        perror("Failed to allocate memory for addresses");
        free(newNode);
        return;
    }
    newNode->addresses[0] = address;
    newNode->address_count = 1;
    newNode->next = head_entry;

/*link the new member to the top of the list*/
    head_entry = newNode;
}

/*A function that creates the ent file for us only if necessary, it goes through the list of entry and for each instance of a name label writes the name of the label and the position in which it appeared in the assembly code*/
void entry_file(char outputFileName[256]) {
    EntryLabel *current;         
    int hasEntryLabels = 0;    
    char entFileName[256];    
    char *dot;                  
    FILE *file;               
	int i;
/** Checking if there are entry labels in the list **/
    current = head_entry;
    while (current != NULL) {
        if (current->address_count > 0) {
            hasEntryLabels = 1;
            break;
        }
        current = current->next;
    }

/** If there are no entry labels, there is no need to create the file **/
    if (!hasEntryLabels) {
        return;
    }

/** Create the new file name with the extension .ent **/
    strcpy(entFileName, outputFileName);
    dot = strrchr(entFileName, '.');
    if (dot != NULL) {
        *dot = '\0'; /** Remove the existing suffix **/
    }
    strcat(entFileName, ".ent");

/** Open the file for writing **/
    file = fopen(entFileName, "w");
    if (file == NULL) {
        perror("Error creating entry file");
        return;
    }

/** writing to file **/
    current = head_entry;
    while (current != NULL) {
        for ( i = 0; i < current->address_count; i++) {
            fprintf(file, "%s %d\n", current->label, current->addresses[i]);
        }
        current = current->next;
    }

 /** closing the file **/
    fclose(file);
}




/** A function that accepts an operand and checks if it is a label and if so it also checks in the list of labels what its address is and what its attribute is according to this it puts it in memory in the appropriate place with the appropriate values ​​and if it is external or entry it puts its instance in the appropriate place in the ext and ent files **/
void find_label(const char *token) {

    LabelEntry *current = head_label;
	int address;
	int i;
/** Initialize the global string to 15 zeros **/
    strcpy(binari_memory5, "000000000000000");
    while (current != NULL) {
        if (strcmp(current->label, token) == 0) {
            if (strcmp(current->attribute, "extern") == 0) {
                addExternalLabel(token, IC);
                binari_memory5[0] = '1';
                return;
            } else if (strcmp(current->attribute, "entry") == 0) {
                addEntryLabel(token, IC);
            }

             address = current->address;
            if (address < 0) {
                address = -address;
                address += IC_copy;
            }

            for ( i = 14; i >= 3; i--) {
                binari_memory5[i] = (address % 2) ? '1' : '0';
                address /= 2;
            }
            binari_memory5[1] = '1';
            return;
        }
        current = current->next;
    }
}


/*A function for the second pass receives a line and sorts it into one of the three types without an operand one operand and two operands there is no operand we will finish the function if there are operands we will look for them and check if they are labels and if so and with the help of the find_label function we will put the labels in memory in the appropriate place*/
int instruction_line_paas_two(const char *line) {
    int addressing_method1 = 4; 
    int addressing_method2 = 4; 
    int address1_value1 = 2048; 
    int address1_value2 = 2048; 

	int i=0;
	int j=0;
    char line_copy[MAX_LINE_LENGTH1];
	char *token;
	int len;
    char processed_line[MAX_LINE_LENGTH1 * 3];
    int result;

 char binari_memory0[15] = "000000000000000";
 char binari_memory1[15] = "000000000000000"; 
 char binari_memory2[15] = "000000000000000"; 
 int binari_memory_flag1 = 0;   
 int binari_memory_flag2 = 0;   



/*add a space before and after the character ',' in the original line*/

	for (j = 0, i = 0; line[i] != '\0'; i++) {
	    if (line[i] == ',') {
		processed_line[j] = '\t';
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
    strncpy(line_copy, processed_line, MAX_LINE_LENGTH1 - 1);
    token = strtok(line_copy, " \t"); /*the first word*/


     len = strlen(token);
    if (token[len - 1] == ':') {/*if the first word is a colon then it is a label statement and we will move to the next word*/
            token = strtok(NULL, " \t");
    }




/*testing if the word is one of the instructions with 2 operands*/
if (strcmp(token, "mov") == 0 ||
    strcmp(token, "cmp") == 0 ||
    strcmp(token, "add") == 0 ||
    strcmp(token, "sub") == 0 ||
    strcmp(token, "lea") == 0) {

	IC++;
/* move to the next word in the line*/
        token = strtok(NULL, " \t");


/*we will check the first operand*/
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
			find_label(token);
			setMemory(IC, binari_memory5);
                        addressing_method1 = 1;

                    }
                }
            }
        }


	IC++;
/*continue to the next word in the line it must be a comma*/

        token = strtok(NULL, " \t");

 /*continue to the next word in the line that is not a comma*/
        token = strtok(NULL, " \t");



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

			find_label(token);
			setMemory(IC, binari_memory5);
                        addressing_method2 = 1;
                    }
                }
            }
        }

	if((addressing_method2==2||addressing_method2==3)&&(addressing_method1==2||addressing_method1==3))return 1;
	IC++;



	return 1;




/*finish mov - lea*/
/*we will advance if there are no operands*/
}else if(strcmp(token, "rts") == 0 ||strcmp(token, "stop") == 0){
	IC++;
	return 1;

/*Test only operand 1*/
}else if (strcmp(token, "clr") == 0 ||
    strcmp(token, "not") == 0 ||
    strcmp(token, "inc") == 0 ||
    strcmp(token, "dec") == 0 ||
    strcmp(token, "bne") == 0 ||
    strcmp(token, "red") == 0 ||
    strcmp(token, "prn") == 0 ||
    strcmp(token, "jsr") == 0 ||
    strcmp(token, "jmp") == 0) {
	IC++;
  /* move to the next word in the line*/
        token = strtok(NULL, " \t");


/*we will check the first operand*/
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
			find_label(token);
			setMemory(IC, binari_memory5);
                        addressing_method1 = 1;
                    }
                }
            }
        }
	IC++;


	
return 1;
}
    return 0;

}

