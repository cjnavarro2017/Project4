/*
	Chris Navarro
	Maurizio Vitale
	===============
	Project 4
	api.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "api.h"

vAddr allocateNewInt(){
	int i, address;
	vAddr newPage;
	
	//finds next unallocated page
	newPage = findPage();
	
	//checks if memory is availbale
	if(newPage == -1){
		printf("NO MEMORY: Exiting...\n");
		exit(0);
	}
	
	//checks if there is any space available
	for(i=0; i < RAM_SIZE; i++){
		if(ram[i] == 0){
			printf("\tMemory Allocated\n");
			ram[i] = 1;
			address = i;
			table[newPage].allocated = 1;
			table[newPage].location = address;
			table[newPage].counter++;
			table[newPage].timeAccessed = difftime(time(0), start);
			return newPage;
		}
	}
	
	//no space is available must run eviction algorithm to make space
	printf("\tRAM FULL. Evicting Page...\n");
	if(setEviction == 1){
		address = evictRAM1(1);
		table[newPage].allocated = 1;
		table[newPage].location = address;
		table[newPage].counter++;
		table[newPage].timeAccessed = difftime(time(0), start);
		printf("\t\tMemory Allocated\n");
	}
	if(setEviction == 2){
		address = evictRAM2();
		printf("\t\tMemory Allocated\n");
	}
	
	return newPage;
}

//initializes all array values to zero
void init_arrays(){
	int i;
	
	for(i=0; i<RAM_SIZE; i++){
		ram[i] = 0;
	}
	
	for(i=0; i<SSD_SIZE; i++){
		ssd[i] = 0;
	}
	
	for(i=0; i<DISK_SIZE; i++){
		disk[i] = 0;
	}
	
	for(i=0; i<PAGE_TABLE; i++){
		table[i].indx = i;
		table[i].lock = 0;
		table[i].allocated = 0;
		table[i].location = -1; 
		table[i].counter = 0; 
	}
	
	return;
}

//grabs next unallocated page
vAddr findPage(){
	int i;
	for(i=0; i<PAGE_TABLE; i++){
		if(table[i].allocated == 0){
			return i;
		}
	}
	
	
	return -1;
	
}

void printPage(struct Page page){
	printf("=============\nPage Index: %d\nAllocated: %d\nLocation: %d\nTime Accessed: %f\n=============\n", 
			page.indx, page.allocated, page.location, page.timeAccessed);
}


//first eviction algorithm - Least Recently Used (LRU)
vAddr evictRAM1(int memory){
	int i;
	int min = -1;
	//finds page with LRU access
	for(i=0; i<PAGE_TABLE; i++){
		if(((-1) < table[i].location && table[i].location < RAM_SIZE) && table[i].lock == 0){
			if(min == -1){
				min = i;
			}
			else{
				if(table[i].timeAccessed < table[min].timeAccessed){
					min = i;
				}
			}
		}
	}
	//NEED to handle shifting the evicted page down
	int loc = table[min].location;
	
	//do i need a lock here
	copy_to_SSD1(&table[min]);
	
	return loc;
	
}

void copy_to_SSD1(struct Page *page){
	int i;
	for(i=0; i<SSD_SIZE; i++){
		if(ssd[i] == 0){
			ssd[i] = 1;
			page->location = i + RAM_SIZE;
			return;
		}
	}
	printf("NO ROOM IN SSD\n");
	return;
}

//first eviction algorithm
vAddr evictRAM2(){
	return 0;
	
}