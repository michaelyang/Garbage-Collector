#include "myalloc.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <signal.h>

void *myalloc2(int size);
void coalesce();
void mark(int *p);
void sweep(int *p);

//helper functions
int *isPtr(int *p);
int blockMarked(int *p);
void markBlock(int *p);
void unmarkBlock(int *p);
int blockAllocated(int *p);
void allocateBlock(int *p);
void unallocateBlock(int *p);
int length(int *p);
int *nextBlock(int *p);
int *start;

void myallocinit(int size) { 
  //i'll give you this to start with, rememebr, it creates  a
  //block of memory for us to use in our psuedo heap
  printf("Attemping to initialize size: %d\n", size);                      
  int blocklen = ((size + 3) / 8 + 1) * 8;  // the next highest multiple of 8
  printf("Actual allocation is of size: %d\n", blocklen);
  start = (int *)malloc(blocklen + 8);     // 8 bytes for the padding, 
                                            // and sentinel header
  if (start == NULL) {
    printf("Cannot init that much space\n");
    exit(1);
  }
  start++;                                 // align on a 4-byte boundary
  *start = blocklen;        //size
  //printf("%d\n",blocklen);
  *(nextBlock(start)) = 1;
}

void printallocation() {
  int *ptr = start;
  int n = 0;
  while((*ptr) != 1){
  	if(blockAllocated(ptr)){
		printf("Block %d\t\tAddress %p\tSize %d\t\tAllocated\n",n, ptr, (*ptr/2)*2);
                n++;
	}
        else {
                printf("Block %d\t\tAddress %p\tSize %d\t\tUnallocated\n",n, ptr, (*ptr/2)*2);
                n++;
        }
        ptr = nextBlock(ptr);

    }
}

void *myalloc(int size) {
  void *ptr;
  ptr = myalloc2(size);
  if(ptr == NULL){
	coalesce();
	ptr = myalloc2(size);
  }
  //now fill in the rest (remember to coalesce if myallco2 does not find a spot
  if(ptr == NULL){
	printf("Not enough space\n");
  }
  return ptr;
}

void *myalloc2(int size) {
  printf("Asked for block of size: %d\n", size);
  int newsize = ((size+3) / 8 + 1) * 8; //best new size
  int tempsize;
  printf("Actual size of block given: %d\n", newsize);
  int *ptr = start;
  int *newptr;
  while((*ptr) != 1){
	if (!blockAllocated(ptr)){
		if (newsize <= *ptr) {
			tempsize = (*ptr) - newsize;
//printf("%d\n",tempsize);
			*ptr = newsize;
			//allocateBlock(ptr);
			newptr = nextBlock(ptr);
			//printf("%d\n",*newptr);
			if(*newptr != 1){
				*newptr = tempsize;
			} 
			allocateBlock(ptr);
			return ptr + 1;
		}
	}
	ptr = nextBlock(ptr);
  }
  return NULL;
}
			

void myfree(void *p) {
  int *pheader = (int *)p-1;
  if(blockAllocated(pheader)){
	unallocateBlock(pheader);
	printf("Block freed!\n");
  }
}

void coalesce() {
  int *ptr = start; //start at the begining header
  int *nextptr; //an extra pointer
  //fill in the rest
  printf("Coalescing\n");
  while(*ptr != 1){ //not the sentinel
	if(!blockAllocated(ptr)){
		nextptr = nextBlock(ptr);
		if(!blockAllocated(nextptr)){
			*ptr = (*ptr/2)*2 + (*nextptr/2)*2;
		}
		else{
			ptr = nextBlock(ptr);

		}
	}
	else{
		ptr = nextBlock(ptr);
	}

  }
} 


void mygc() {
  //int **max = (int **) 0xbfffffffUL;   // the address of the top of the stack
  unsigned long stack_bottom;
  int **max =  (int **) GC_init();   // get the address of the bottom of the stack
  int* q;
  int **p = &q;    // the address of the bottom of the stack
  
  while (p < max) {
    //printf("0. p: %u, *p: %u max: %u\n",p,*p,max);
    mark(*p);
    p++;
  }

  //now fill in the rest think sweep , think coalesce
  sweep(start);
  coalesce();
  return;
}



void mark(int *p) {
  int i;
  int *ptr;
  //printf("one\n");
  if(!(ptr = isPtr(p))){return;}
  //printf("two\n");
  if(blockMarked(ptr)){return;}
  printf("Marking address: %p\n", p);
  if(blockAllocated(ptr)){
	markBlock(ptr);
  }
  for(i = 0; i < length(ptr); i++){
	mark((int *)ptr[i]);
  }
  return;  
}

void sweep(int *ptr) {
  while(*ptr != 1){
	if(blockMarked(ptr)){
//printf("GOTHERE\n");
		unmarkBlock(ptr);
	}
	else if(blockAllocated(ptr)){
		printf("sweeping address: %p\n", ptr);
		unallocateBlock(ptr);
	}
	ptr = nextBlock(ptr);
  }
  return;
}

int *isPtr(int *p) {  
 // this is a very inefficient algorithm
 //return the pointer or NULL
 // printf("address of p is: %p\n", p);
  int *ptr = start;
  while( *ptr != 1 ){
	//printf("address of header being checked: %p\n", ptr);
	if((ptr < p) && (p < nextBlock(ptr))){
		return ptr;
	}
	ptr = nextBlock(ptr);
  }		
  return NULL;
}


int blockMarked(int *p) {
  if ((*p % 4) > 1){
	return 1;
  }
  return 0;
}

void markBlock(int *p) {
  *p += 2;	
}

void unmarkBlock(int *p) {
  *p -= 2;
}

int blockAllocated(int *p) {
  return *p % 2;
}

void allocateBlock(int *p) {
  *p += 1; 
}

void unallocateBlock(int *p) {
  *p -= 1;	 
}

int *nextBlock(int *p) {
  return p+((*p)/4);
}

int length(int *p) {
  return ((*p)/4);
}
