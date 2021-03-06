/*
	Chris Navarro
	Maurizio Vitale
	===============
	Project 4
	main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "api.h"


void memoryMaxer() {
	vAddr indexes[30];
	for (int i = 0; i < 30; ++i) {
		indexes[i] = allocateNewInt();
		printf("allocated\n");
		//printPage(table[i]);
		int *value = accessIntPtr(indexes[i]); *value = (i * 3); 
		unlockMemory(indexes[i]);
		//printPage(table[i]);
		printf("accessed value\n");
	}
	for(int i =0; i<30;i++){
		printf("INDEX %d\n", i);
		printPage(table[i]);
	}
	
	for (int i = 0; i < 30; i++) {
		freeMemory(indexes[i]);
	}
}

int main(int argc, char *argv[]){
	//start clock
	start = time(0);
	
	//checks if valid usage
	if(argc != 2){
		printf("Correct Usage is: ./main #\nEXITING PROGRAM\n");
		exit(0);
	}
	
	//sets eviction type
	setEviction = atoi(argv[1]);
	
	//inits memory arrays to zero
	init_arrays();
	//=====Tests======
	int i;
	// vAddr index[100];
	// for(i =0; i<26;i++){
	// 	printf("Allocating Memory for index %d\n", i);
	// 	index[i] = allocateNewInt();
	// }
	
	memoryMaxer();
	
	for(i =0; i<30;i++){
		printf("INDEX %d\n", i);
 		printPage(table[i]);
	}

	return 0;
} 
