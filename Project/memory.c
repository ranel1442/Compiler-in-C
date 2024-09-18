/*The purpose of the file is to handle and declare everything related to the program's memory and its pointers*/
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 61440 /*15 bits and 4096 cells*/ 

unsigned char memory[MEMORY_SIZE]; /* The memory of the program */
unsigned char memory_data[MEMORY_SIZE]; /* The data memory */

int IC = 100; /*The instruction counter is initialized to 100*/ 
int DC = 0;   /*The data counter is initialized to 0*/ 


/**A function to define a value in memory at a given address using an array of characters The function receives the address and an array that we would like to insert into memory
 */
void setMemory(int address, const char value[15]) {
    int baseAddress = address * 15;  /*Calculation of the starting address*/ 
	int i;
    for ( i = 0; i < 15; i++) {
        memory[baseAddress + i] = value[i];  /*Entering the values ​​into memory*/ 
    }
}

/**A function to define a value in data memory at a given address using an array of characters The function receives the address and an array that we would like to insert into memory
 */
void setMemory_data(int address, const char value[15]) {
    int baseAddress = address * 15;  /*Calculation of the starting address*/ 
	int i;
    for (i = 0; i < 15; i++) {
        memory_data[baseAddress + i] = value[i];/*Entering the values ​​into data memory*/ 
    }
}




/*A function to receive a desired value from the memory receives the address you want to receive and returns it (the function is not used in the program but is important to have behind the scenes for debugging)*/
unsigned short getMemory(int address) {
    int bitIndex = address * 15; /* Starting bit index of the cell */
    unsigned short value = 0;
	int i;
    for (i = 0; i < 15; i++) {
        if (memory[bitIndex / 8] & (1 << (bitIndex % 8))) {
            value |= (1 << i);
        }
        bitIndex++;
    }
    return value;
}

/*A function to print the memory of the program (a function not used in the program but important to have for debugging)
 */
void printMemory() {
    int endAddress = (IC+DC) * 15;  /*Calculation of the final address*/ 
	int i;
    printf("Printing memory from address 1500 to %d:\n", endAddress);

    for ( i = 1500; i <= endAddress; i++) {
        printf("memory[%d] = %c\n", i, memory[i]);
    }
}


/*A function to connect the data memory and the memory of the program and check that we have not exceeded the maximum size of the program*/
int memory_connection() {
	int i;
    /* Checking if IC + DC exceed the size of the memory */
    if ((IC + DC) > 3995) {
        printf("Error: Memory overflow, IC + DC exceeds 3995.\n");
        return 0; 
    }

    /* Copying the content from memory_data to memory*/
    for ( i = 0; i <= DC*15; i++) {
        memory[IC*15+ i] = memory_data[i];
    }

    return 1;  
}




