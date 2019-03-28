#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define MAXCHAR 460000
#define THREADS 100		//CHANGE ME!

typedef struct thread_info {		/* Used as argument to doSomething() */
    pthread_t 	id;			/* ID returned by pthread_create() */
    int		num;			/* Application-defined thread # */
    int		start_index;		/* Index from witch doSomething shall start from */
    int		length;			/* Number of characters to cypher */
};

clock_t before;
clock_t after;
double total;

pthread_t tid[THREADS];
char text_buffer[THREADS][MAXCHAR];
char montext[MAXCHAR];
struct thread_info tinfo[THREADS];

char encrypt(int key, char ch)
{    
    if(ch >= 'a' && ch <= 'z')
    {
	ch = ch + key;
	if (ch > 'z')
	{
	    ch = ch - 'z' + 'a' - 1;
	}
    }
    else if(ch >= 'A' && ch <= 'Z')
    {
	ch = ch + key;
	if (ch > 'Z')
	{
	    ch = ch - 'Z' + 'A' - 1;
	}
    }
    return ch;
}

void* doSomeThing(void *arg)
{
    int i = 0;
    struct thread_info *tinfo = arg;

    tinfo->id = pthread_self();
    //printf("Th#%d created successfully\n", tinfo->num);
    fflush(stdout);
    
    //DO SOMETHING
    for(int i = tinfo->start_index; i < (tinfo->start_index + tinfo->length); i++)
    {
	montext[i] = encrypt(1, montext[i]);
    }
    //for(int i = 0; i < 0xFFFFFF; i++);	//add a time consumming task so one can see the concurrency
        
    //printf("Th#%d done \n", tinfo->num);
    fflush(stdout);
    pthread_exit(NULL);
    return NULL;
}

int main(void)
{
    before = clock();
    strcpy(montext, "");
    char output[MAXCHAR];
    
    //opening file to cypher
    int i,j, count = 0;
    int err;
    FILE *fp;
    char str[MAXCHAR];
    char* filename = "./Input";

    fp = fopen(filename, "r");
    if (fp == NULL) {
	printf("Could not open file %s\n", filename);
	return 1;
    }
    //printf("--Opening file:\n");
    
    //Concatenate text to a single string
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
	strcat(montext, str);
	int len = strlen(str);
	//printf("%d :%s \n", len, str);
	count = count + len;
    }
    //printf("Count is: %d \n", count);
    //printf("%s \n", montext);
    fclose(fp);
    
    //Calculating optimal max load for the threads
    int thread_load = count/THREADS;
    int remainder = count%THREADS;
    
    if(remainder != 0)
    {
	thread_load += 1;
    }
    //printf("Char/thread: %d \n", (thread_load));

    
    //Creating Threads
    j = 0;
    for (i=0; i < THREADS; i++)
    {	
	tinfo[i].num = i;
	tinfo[i].start_index = j;
	int length = thread_load;
	if (i < remainder)
	{
	    length += 1;
	}
	tinfo[i].length = length;
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, &tinfo[i]);
        if (err != 0)
            printf("Can't create thread :[%s]\n", strerror(err));
	//printf("length: %d\n", length);
	j += length;
    }
    
    //Waiting for all the working threads to be done
    for (i=0; i<THREADS; i++)
    {
	pthread_join(tid[i], NULL);
    }
    
    //Concatenate text_buffer's rows back to one single string
    //printf("OUTPUT: \n%s", montext);
    after = clock();
    total = ((double) (after - before))/ (CLOCKS_PER_SEC);
    printf("THREAD: TOTAL TIME: %lf\n",total);
    return 0;
}
