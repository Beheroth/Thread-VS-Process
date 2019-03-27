#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define MAXCHAR 16384
#define THREADS 6
#define KEY 1

pthread_t tid[THREADS];
char text_buffer[THREADS][MAXCHAR];
char montext[MAXCHAR];

void encrypt(int key, int id)
{
    char ch;
    int i;
    char* message = montext;
    
    //printf("Encrypt: %s\n", message);
    for(i = 0; message[i] !='\0'; ++i)
    {
	ch = message[i];
	if(ch >= 'a' && ch <= 'z')
	{
	    ch = ch + key;
	    if (ch > 'z')
	    {
		ch = ch - 'z' + 'a' - 1;
	    }

	    message[i] = ch;
	}
	else if(ch >= 'A' && ch <= 'Z')
	{
	    ch = ch + key;
	    if (ch > 'Z')
	    {
		ch = ch - 'Z' + 'A' - 1;
	    }
	    message[i] = ch;
	}
    }
    printf("Th#%d: %s\n", id, message);
    fflush(stdout);
}

char Encrypt(int key, int id, char ch)
{    
    //printf("Encrypt: %s\n", message);
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
    unsigned long i,j = 0;

    pthread_t id = pthread_self();
    
    //finding index in text_buffer
    while(j < THREADS && !pthread_equal(id,tid[j]))
    {
	j++;
    }

    if(pthread_equal(id,tid[j]))
    {	
        printf("Th#%lu processing\n", j);
	fflush(stdout);
    }
    
    //DO SOMETHING
    encrypt(1, j);
    for(int i = 0; i < 0xFFFFFF; i++);	//add a time consumming task so one can see the concurrency
    for(
    printf("Th#%lu done \n", j);
    fflush(stdout);
    pthread_exit(NULL);
    return NULL;
}

int main(void)
{
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
    printf("--Opening file:\n");
    
    //Concatenate text to a single string
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
	strcat(montext, str);
	int len = strlen(str);
	count = count + len;
	printf("%d :%s \n", len, str);
    }
    printf("Count is: %d \n", count);
    printf("%s \n", montext);
    fflush(stdout);
    fclose(fp);
    
    //Calculating optimal max load for the threads
    int thread_load = count/THREADS;
    if(count%THREADS != 0)
    {
	thread_load += 1;
    }
    printf("Char/thread: %d \n", (thread_load));
    
    //Filling text_buffer
    int buffer_index = 0;
    while(buffer_index < THREADS)
    {
	int k = 0;
	while(k < thread_load && montext[k + (buffer_index*(thread_load))] != '\0')
	{
	    //printf("index: %d \n", k + (buffer_index*(thread_load)));
	    text_buffer[buffer_index][k] = montext[k + (buffer_index*(thread_load))];
	    k++;
	}
	text_buffer[buffer_index][k] = '\0';
	printf("-%s-\n", text_buffer[buffer_index]);
	buffer_index++;
    }

    //Creating Threads
    while(i < THREADS)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
	
        if (err != 0)
            printf("Can't create thread :[%s]\n", strerror(err));
        else
	{
            printf("Thread #%d created successfully\n", i);
	    fflush(stdout);
	    i++;
	}
    }
    
    //Wainting for all the working threads to be done
    for (i=0; i<THREADS; i++)
    {
	pthread_join(tid[i], NULL);
    }
        
    //All the thread should be finished by now
    printf("text_buffer: \n");
    for(i=0; i < THREADS; i++)
    {
	printf("%s", text_buffer[i]);
    }

    //Concatenate text_buffer's rows back to one single string
    strcpy(output, "");
    for (i = 0; i < THREADS; i++)
    {
	strcat(output, text_buffer[i]);
    }
    printf("OUTPUT: \n%s", output);
    return 0;
}
