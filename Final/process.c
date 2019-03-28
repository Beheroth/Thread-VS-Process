#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXCHAR 460000
#define PROC 100		//CHANGE ME

clock_t before;
clock_t after;
double total;

pid_t pid;
pid_t fork(void);
pid_t wait(int* status);
pid_t wait(int* done);

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


int main()
{
	before = clock();
	//Create mytext variable
	char mytext[MAXCHAR];
	strcpy(mytext, "");
	
	char output[MAXCHAR];
	strcpy(output, "");

	//Open file to cypher
	FILE *fp;
	char str[MAXCHAR];
	char *filename = "./Input";

	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		//printf("Could not open file %s\n", filename);
		return 1;
	}
	//printf("Opening file :\n");

	int count = 0;	
	while(fgets(str, MAXCHAR, fp)!=NULL){ 
		strcat(mytext, str);
		int len = strlen(str);
		count = count + len;
		//printf("%d :%s \n", len, str);
	}
	//printf("Count is: %d \n", count);
    //printf("%s \n", mytext);
    fflush(stdout);
	fclose(fp);
	
	// ----------- Répartition travail -------------
	
	//Calculate optimal load of the processus
	int proc_load = count/PROC;
	int remainder = count%PROC;

	//printf("proc_load = %d\nremainder = %d\n", proc_load, remainder);
    
	// ------------------ SHM -----------------

	// Initialization of the shared buffer

	// Our memory segment is named "buff"
	// Create the segment
	// Create a str from the text
	char *shm;
	int shmid;
	if((shmid = shmget(IPC_PRIVATE, sizeof(char)*MAXCHAR*PROC, IPC_CREAT | 0666)) < 0 )
	{
		perror("shmget");
		exit(1);
	}
	//printf("shmid: %d\n", shmid);
	// Attach the segment to our data space
	shm = shmat(shmid, NULL, 0);
	if((char *) shm  == NULL)
	{
		perror("shmat error");
		exit(1);
	}
	*shm = *mytext;
	//printf("Attached\n");
	//printf("Contenu de la mémoire : %d\n", shmids);	
	
	// ------------------ PROCESSUS ---------------------
	int fils = 0;
	int b = 0;
	int flag = 0;
	int status;
	int q =0;
	int start_index =0, length;

	//printf("--Creating processes\n");
	while(b != PROC){
		pid_t pid = fork();
		length = proc_load;
		if(b < remainder)
		{
			length = length + 1;
		}
		
		if(pid == 0)
		{
			fils = 1;
			break;
		}

		else
		{
			//printf("Parent: b: %d, start_index: %d, length: %d\n", b, start_index, length);
			start_index = start_index + length;		
		}
		b++;
	}

	if(fils == 1)
	{
		pid = getpid();
		//printf("Child: b: %d, start_index: %d, length: %d\n", b, start_index, length);
		//printf("Valeur incrémentation tableau: %d \nTraite: %s ",b, text_buffer[b]);
		for(int i=start_index; i < length + start_index;i++)
		{
			shm[i] = encrypt(1, mytext[i]);
		}
		//printf("\nChild: Text_buffer[%d]:\n%s", b, mytext);
		//memcpy(&shm + MAXCHAR*b, text_buffer[b], sizeof(text_buffer[b]));

	}

	else
	{
		while(q < PROC)
		{
			int w = wait(&status);
			q++;
		}	
		printf("--Recomposition:\n");	
		fflush(stdout);	
		strcat(output, shm);
		//printf("OUTPUT:\n%s", output);
		shmdt (shm);
		shmctl(shmid,IPC_RMID,0);
	}
	after = clock();
    total = ((double) (after - before))/ (CLOCKS_PER_SEC);
    printf("PROCESS: TOTAL TIME: %lf\n",total);

return 0;
}



