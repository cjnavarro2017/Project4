/*
	Chris Navarro
	Maurizio Vitale
	===============
	Project 4
	api.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "api.h"

vAddr allocateNewInt(){
	vAddr newpage;
	newpage = findPage();
	usleep(250);
	printf("Allocating memory with virtual address: %d\n", newpage);
	table[newpage].lock = 0;
	table[newpage].allocated = 1;
	table[newpage].location = 0;
	table[newpage].counter = 0;
	table[newpage].timeAccessed = 0;
	
	swap_to_ram(0, newpage);
	
	return newpage;
}

int * accessIntPtr (vAddr address) {
	table[address].lock = 1;
	struct timeval tv;
	
	printf("Accesssing adress: %d\n", address);
	
	int location = table[address].location ;
	int loc_index = table[address].indx;
	
	gettimeofday(&tv,NULL);
	
	table[address].timeAccessed = tv.tv_sec;
	
	switch(location) {
		case 0:
			return &ram[loc_index];
			break;
		case 1:
			swap_to_ram(ssd[loc_index], address);
			sleep(1);
			accessIntPtr(address);
			break;
		case 2:
			swap_to_disk(disk[loc_index], address);
			accessIntPtr(address);
			break;
	}
	
	return NULL;
}

void swap_to_ram(int value, vAddr address){
	int i,flag;
	int victim;
	int temp;
	flag = 1;
	//check if room
	for(i = 0; i < RAM_SIZE; i++){
		if(ram[i] == -1){
			usleep(250);
			ram[i] = value;
			table[address].location = 0;
			table[address].indx = i;
			flag = 0;
			break;
		}
	}
	//eviction needed
	if(flag == 1){
		//LRU
		if(setEviction == 1){
			usleep(250);
			victim = lru_evict(0);
			
			//tempval = ram[table[victim].indx];
			
			temp = table[address].location;
			table[address].location = table[victim].location;
			table[victim].location = temp;
			
			//temp = table[address].indx;
			//table[address].indx = table[victim].indx;
			//table[victim].indx = temp;
			
			temp = ram[table[address].indx];
			ram[table[address].indx] = ram[table[victim].indx];
			ram[table[victim].indx] = temp;
					
		}
		//LFU
		else{
			usleep(250);
			lfu_evict(0);
		}	
	}
	
}

void swap_to_ssd(int value, vAddr address){
	int i,flag, temp;
	vAddr victim;
	flag = 1;
	//check if room
	for(i = 0; i < SSD_SIZE; i++){
		if(ssd[i] == -1){
			usleep(250);
			ssd[i] = value;
			table[address].location = 1;
			table[address].indx = i;
			flag = 0;
			break;
		}
	}
	//eviction needed
	if(flag){
		//LRU
		if(setEviction == 1){
			usleep(250);
			victim = lru_evict(1);
			
			//temp = ram[table[victim].indx]; < no
			
			
			temp = table[address].location;
			table[address].location = table[victim].location;
			table[victim].location = temp;
			
			//temp = table[address].indx;
			//table[address].indx = table[victim].indx;
			//table[victim].indx = temp;
			
			temp = ssd[table[address].indx];
			ssd[table[address].indx] = ssd[table[victim].indx];
			ssd[table[victim].indx] = temp;
		}
		//LFU
		else{
			usleep(250);
			victim = lfu_evict(1);
		}		
	}
	
}

void swap_to_disk(int value, vAddr address){
	int i,flag, temp;
	vAddr victim;
	flag = 1;
	//check if room
	for(i = 0; i < DISK_SIZE; i++){
		if(disk[i] == -1){
			usleep(2500);
			disk[i] = value;
			table[address].location = 0;
			table[address].indx = i;
			flag = 0;
			break;
		}
	}
	//eviction needed
	if(flag){
		//LRU
		if(setEviction == 1){
			usleep(2500);
			victim = lru_evict(2);
			
			//temp = disk[table[victim].indx]; < stop
			
			
			temp = table[address].location;
			table[address].location = table[victim].location;
			table[victim].location = temp;
			
			//temp = table[address].indx;
			//table[address].indx = table[victim].indx;
			//table[victim].indx = temp;
			
			temp = ssd[table[address].indx];
			disk[table[address].indx] = disk[table[victim].indx];
			disk[table[victim].indx] = temp;
			
		}
		//LFU
		else{
			usleep(2500);
			victim = lfu_evict(2);
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

//EVICTION ALGO

//lru
vAddr lru_evict(int memory){
	int i;
	vAddr max = -1;
	
	for(i = 0; i < PAGE_TABLE; i++){
		if(table[i].location == memory && table[i].lock == 0){
			if(max == -1){
				max = i;
			}
			else{
				if(table[i].timeAccessed > table[max].timeAccessed){
					max = i;
				}
			}	
		}
		
	}
	// if(max == -1){
	// 	printf("NO ROOM, FULL");
	// 	exit(0);
	// }
	return max;
}

//lru
vAddr lfu_evict(int memory){
	int i;
	vAddr min = -1;
	
	for(i = 0; i < PAGE_TABLE; i++){
		if(table[i].location == memory && table[i].lock == 0){
			if(min == -1){
				min = i;
			}
			else{
				if(table[i].counter <  table[min].counter){
					min = i;
				}
			}	
		} 
		
	}
	return min;
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
