//Lab 2 Week 2 C code file

#include "Lab2-Part2.h"

void main()
{
	//allocating  args and thread 
	struct Args *args = (struct Args*)malloc(sizeof(struct Args));
	args->size = 128;
	pthread_t threads[NUM_THREADS];	
	
	int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);	// returns a file descriptor
	struct itimerspec itval;	// structure to hold period and starting time
	
	//Starting the real time part that is not taguht in lecture but we are expected to know with no direction or help except vague instructions during the beginning of the lab
	// timer fires every ## sec, ## nsec
    	itval.it_interval.tv_sec = 5;		// check the data type
    	itval.it_interval.tv_nsec = 5;	// check the data type
    	
    	// Timer will start in ## sec, ## nsec from the moment the timer is started
    	itval.it_value.tv_sec = 5;		// check the data type
    	itval.it_value.tv_nsec = 5;		// check the data type
    	
    	timerfd_settime(timer_fd, 0, &itval, NULL);	// this function has a return value

	//file ptrs
	FILE *first_file = fopen("first.txt", "r");
	if (first_file == NULL) {
       		printf("Error: file pointer is null.");
       		exit(1);
   	}
	FILE *second_file = fopen("second.txt", "r");
	if (second_file == NULL) {
       		printf("Error: file pointer is null.");
       		exit(1);
   	}
   	
   	int i = 0;
   	
   	struct sched_param param;
   	
   	// Declare ourself as a real time task by elevating our priority
	// and setting an appropriate scheduling policy.
	param.sched_priority = MY_PRIORITY;
	sched_setscheduler(0, SCHED_FIFO, &param);	// this function has a return value
		// Always good to check for errors. Look for sched_setscheduler()
   
   	 //reading from file
   	 while(!feof(first_file))
   	 {  	
   	 	pthread_create(&threads[1], NULL, readLine, first_file);
   	 	pthread_join(threads[1], NULL);
   	 	args->row = i;
   	 	pthread_create(&threads[3], NULL, buffer_to_global, (void *)args);
   	 	pthread_join(threads[3], NULL);
   	 	i++;
   	 	pthread_create(&threads[2], NULL, readLine, second_file);
   	 	pthread_join(threads[2], NULL);
   	 	args->row = i;
   	 	pthread_create(&threads[3], NULL, buffer_to_global, (void *)args);
   	 	pthread_join(threads[3], NULL);
   	 	i++;
   	 }
   	 
   	 //print result of global array
   	 print(global);
   	 free(args); //free args
   	 pthread_exit(NULL); //causes program not to seg fault
}

//reads line from file, for threads one and two
void *readLine(void * fp)
{
	FILE * file = (FILE *)fp;
    	//line buffer
   	 int maximumLineLength = 128;
   	 char *lineBuffer = (char *)malloc(sizeof(char) * maximumLineLength);

   	 if (lineBuffer == NULL) {
    	   	printf("Error allocating memory for line buffer.");
    	   	exit(1);
    	}

   	 char ch = getc(file);
   	 int count = 0;

    	while ((ch != '\n') && (ch != EOF)) {
       		if (count == maximumLineLength) {
       			maximumLineLength += 128;
       			lineBuffer = realloc(lineBuffer, maximumLineLength);
            		if (lineBuffer == NULL) {
                		printf("Error reallocating space for line buffer.");
                		exit(1);
            }
        }
        lineBuffer[count] = ch;
        count++;

        ch = getc(file);
   	 }

	lineBuffer[count] = '\0';
    	char line[count + 1];
    	strncpy(line, lineBuffer, (count + 1));
    	free(lineBuffer);
    	const char *constLine = line;
    	//return constLine;
    	//writing to global buffer
    	strncpy(buffer, constLine, (count+1));
}

//third thread function to cpy from buffer to global
void *buffer_to_global(void * args)
{
	struct Args* arg_funct = (struct Args*) args;
	int row = arg_funct->row;
	int size = arg_funct->size;
	strncpy(global[row], buffer, size);
}

//printing global array
void print(char a[][128])
{
    printf("The gloabl array says: \n");
    for (int i = 0; i < 16; i++){
        printf("%s\n",a[i]);
    }        
}