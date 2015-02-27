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
	
}

int * accessIntPtr (vAddr address) {
	table[address].lock = 1;
	
	int location = table[address].location ;
	int loc_index = table[address].indx;
	
	switch(location) {
		case 0:
			return &ram[loc_index];
			break;
		case 1:
			swap_to_ram(&ssd[loc_index], address);
			accessIntPtr(address);
			break;
		case 2:
			swap_to_disk(&disk[loc_index], address);
			accessIntPtr(address);
			break;
	}
	
	return NULL;
}

void swap_to_ram(int *value, vAddr address){
	int i,flag;
	int victim;
	flag = 1;
	//check if room
	for(i = 0; i < RAM_SIZE; i++){
		if(ram[i] == -1){
			usleep(250);
			ram[i] = value;
			table[address].location = 0;
			table[address].indx = i;
			flag = 0;
		}
	}
	//eviction needed
	if(flag){
		//LRU
		if(setEviction == 1){
			usleep(250);
			lru_evict(0;)
			
		}
		//LFU
		else{
			usleep(250);
			
		}		
	}
	
}

void swap_to_disk(int *value, vAddr address){
	int i,flag;
	int victim;
	flag = 1;
	//check if room
	for(i = 0; i < RAM_SIZE; i++){
		if(ram[i] == -1){
			usleep(250);
			ram[i] = value;
			table[address].location = 0;
			table[address].indx = i;
			flag = 0;
		}
	}
	//eviction needed
	if(flag){
		//LRU
		if(setEviction == 1){
			usleep(250);
			lru_evict(0;)
			
		}
		//LFU
		else{
			usleep(250);
			
		}		
	}
	
}



//unlocks page indicating it can be swapped
void unlockMemory (vAddr address) {
    //function incomplete, I think we have to move it to RAM before anything else.
	if(table[address].location != 0){
		printf("trying to unlock memory thats not in ram!");
		exit(0);
	}
	
	table[address].lock = 0;
}

//user is done with memory page
void freeMemory(vAddr address){
	if(table[address].lock == 0){
        int location = table[address].location;
		//in ram
        switch(location) {
            case 0:
            	ram[table[address].indx] = 0;
            	table[address].location = -1;
				table[address].allocated = 0;
            	break;
            case 1:
            	ssd[table[address].indx] = 0;
            	table[address].location = -1;
				table[address].allocated = 0;
            	break;
            case 2:
            	disk[table[address].indx] = 0;
            	table[address].location = -1;
				table[address].allocated = 0;
            	break;
        }
        printf("Address is freed!\n");
	}
	else{
		printf("Attempting to free a locked page!\n");
	}
}

//initializes all array values to zero
void init_arrays(){
	int i;
	
	for(i=0; i<RAM_SIZE; i++){
		ram[i] = -1;
	}
	
	for(i=0; i<SSD_SIZE; i++){
		ssd[i] = -1;
	}
	
	for(i=0; i<DISK_SIZE; i++){
		disk[i] = -1;
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

//prints a pages information
void printPage(struct Page page){
	printf("=============\nPage Index: %d\nAllocated: %d\nLocation: %d\nTime Accessed: %f\n=============\n", 
			page.indx, page.allocated, page.location, page.timeAccessed);
}
