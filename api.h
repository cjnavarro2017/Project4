/*
	Chris Navarro
	Maurizio Vitale
	===============
	Project 4
	api.h
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef API
#define API

//definitions for memory sizes
#define RAM_SIZE 25
#define SSD_SIZE 100
#define DISK_SIZE 1000

#define PAGE_TABLE 1000

//typedef
typedef signed short vAddr;

//page struct definition
struct Page{
		
	//page information
	int indx; //index location
	int lock; //lock status: 0 - unlocked, 1 - locked
	int allocated; //is page allocated: 0 - No, 1 - Yes
	int location; //where is the page: 0 - Memory, 1 - SSD, 2 - Harddrive, -1 - Non-existent
	int counter; //increments with every memory access
	double timeAccessed; // records last time page was accessed
	};
	

struct Page table[PAGE_TABLE];//page table

//memory arrays
int ram[RAM_SIZE];
int ssd[SSD_SIZE];
int disk[DISK_SIZE];

//general globals
int setEviction;//eviction type to use: 1 or 2

time_t start;//begins clock





//prototypes
vAddr allocateNewInt();
int * accessIntPtr(vAddr address);
void unlockMemory(vAddr address);
void freeMemory(vAddr address);

void init_arrays();
vAddr findPage();
void printPage();



#endif
