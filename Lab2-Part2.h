//Lab 2 week 2 header file

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <sys/timerfd.h>

#define NUM_THREADS 3
#define MY_PRIORITY 51  // kernel is priority 50

//global string array and allocating its memory
char global[20][128]; //rows of 128 characters
//global buffer for reading from file 
char buffer[256]; //256 chars per line

//structs for args
struct Args {
	int row;
	int size;
};

//declarations
void *readLine(void *fp);
void *buffer_to_global(void * args);
void print(char (*a)[128]);