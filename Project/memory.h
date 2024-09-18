#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 61440 

extern unsigned char memory[MEMORY_SIZE];
extern unsigned char memory_data[MEMORY_SIZE]; 

extern int IC; 
extern int DC;  

void initializeMemory();

void setBit(unsigned char *memory, int index, int value);

void setMemory(int address, const char value[15]);

void setMemory_data(int address, const char value[15]);

unsigned short getMemory(int address);

void printMemory();

int memory_connection(void);

#endif /* MEMORY_H */

