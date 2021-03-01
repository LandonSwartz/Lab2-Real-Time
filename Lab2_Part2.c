//Lab 2 Week 2 C code file

#include "Lab2-Part2.h"

int row = 0;

void main()
{
	//allocating  args and thread 
	struct Args *argsFirst = (struct Args*)malloc(sizeof(struct Args));
	struct Args *argsSecond = (struct Args*)malloc(sizeof(struct Args));
	struct Args *argsThird = (struct Args*)malloc(sizeof(struct Args));
	argsFirst->size = 128;
	argsSecond->size = 128;
	argsThird->size = 128;
	argsThird->row = 0;
	strcpy(argsFirst->filename, "first.txt");
	strcpy(argsSecond->filename, "second.txt");
	pthread_t threads[NUM_THREADS];	
	int ret; //for return values
	
	struct sched_param param;
   	
	ret = sched_setscheduler(threads[0], SCHED_FIFO, &param);	// this function has a return value
	ret = sched_setscheduler(threads[1], SCHED_FIFO, &param);	// this function has a return value
	ret = sched_setscheduler(&threads[2], SCHED_FIFO, &param);	// this function has a return value
	if(ret == -1)
	{
		printf("Error with scheduler\n");
	}
   
	ret = pthread_create(&threads[0], NULL, threadFunction, (void*) argsFirst);
	if(ret)
	{
		printf("create pthread failed horribly\n");
		return;
	}
	ret = pthread_create(&threads[2], NULL, threadFunction3, (void*) argsThird);
	ret = pthread_create(&threads[1], NULL, threadFunction2, (void*) argsSecond);
   	 
   	ret = pthread_join(threads[0], NULL);
   	if(ret)
   		printf("join thread failed more horribly\n");
   	pthread_join(threads[1], NULL);
   	pthread_join(threads[2], NULL);

   	 
   	 //print result of global array
   	 print(global);
   	 free(argsFirst); //free args
   	 free(argsSecond);
   	 free(argsThird);
   	 pthread_exit(NULL); //causes program not to seg fault
}

//function of real periodic task
void *threadFunction(void *args)
{
	struct Args* arg_funct = (struct Args*) args;
	struct period_info pinfo;
	
	periodic_task_init2(&pinfo);
	
	//opening file
	printf("filename = %s\n", arg_funct->filename);
	FILE *file = fopen(arg_funct->filename, "r");
	if (file == NULL) {
       		printf("Error: file pointer is null.");
       		exit(1);
   	}
   	
   	while (1) {
                readLine(file);
                wait_rest_of_period(&pinfo);
                printf("Test1\n");
       }	
       
       return NULL;
   	
}

void *threadFunction2(void *args)
{
	struct Args* arg_funct = (struct Args*) args;
	struct period_info pinfo;
	
	periodic_task_init2(&pinfo);
	
	//opening file
	printf("filename = %s\n", arg_funct->filename);
	FILE *file = fopen(arg_funct->filename, "r");
	if (file == NULL) {
       		printf("Error: file pointer is null.");
       		exit(1);
   	}
   	
   	while (1) {
                readLine(file);
                wait_rest_of_period(&pinfo);
                printf("Test 2\n");
       }	
       
       return NULL;
}

//waiting for rest of period of function
static void wait_rest_of_period(struct period_info *pinfo)
{
        inc_period(pinfo);
 
        /* for simplicity, ignoring possibilities of signal wakes */
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

//initializing a periodic task
static void periodic_task_init(struct period_info *pinfo)
{
        /* for simplicity, hardcoding a 1ms period */
        pinfo->period_ns = 1000000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

//initializing a periodic task
static void periodic_task_init2(struct period_info *pinfo)
{
        /* for simplicity, hardcoding a 1ms period */
        pinfo->period_ns = 1000000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

void *threadFunction3(void *args)
{
	struct Args* arg_funct = (struct Args*) args;
	struct period_info pinfo;
	
	periodic_task_init2(&pinfo);
   	
   	while (1) {
                buffer_to_global(args);
                arg_funct->row++;
                wait_rest_of_period(&pinfo);
                printf("Test 3\n");
                print(global);
       }	
       
       return NULL;
   	
}

//incrementing period
static void inc_period(struct period_info *pinfo) 
{
        pinfo->next_period.tv_nsec += pinfo->period_ns;
 
        while (pinfo->next_period.tv_nsec >= 1000000000) {
                /* timespec nsec overflow */
                pinfo->next_period.tv_sec++;
                pinfo->next_period.tv_nsec -= 1000000000;
        }
}

//reads line from file, for threads one and two
void *readLine(FILE * fp)
{	
	FILE * file = fp;
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