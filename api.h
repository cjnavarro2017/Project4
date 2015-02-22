/*
	Chris Navarro
	Maurizio Vitale
	===============
	Project 4
	api.h
*/

#include <stdio.h>
#include <stdlib.h>

//definitions for memory sizes
#define MEM_SIZE 25
#define SSD_SIZE 100
#define HD_SIZE 1000

#define PAGE_TABLE 1000

//typedef
typedef signed short vAddr;

//page struct definition
struct Page{
		
	//page information
	int lock; //lock status
	int allocated; //is page allocated
	int location; //where is the page: 0 - Memory, 1 - SSD, 2 - Harddrive, -1 - Non-existent
	int counter; //increments with every memory access
	};
	

struct Page table[PAGE_TABLE];







//prototypes
vAddr allocateNewInt();
int * accessIntPtr(vAddr address);
void unlockMemory(vAddr address);
void freeMemory(vAddr address);