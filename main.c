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
	vAddr index[100];
	for(i =0; i<26;i++){
		printf("Allocating Memory for index %d\n", i);
		index[i] = allocateNewInt();
	}
	for(i =0; i<26;i++){
		printPage(table[i]);
	}

	return 0;
} 
