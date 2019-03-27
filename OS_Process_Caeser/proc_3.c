
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 16384
#define text_size 0
#define PROC 6
#define KEY 1


pid_t pid;
pid_t fork(void);
pid_t wait(int* status);
pid_t wait(int* done);
char text_buffer[PROC][MAXCHAR];

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
	//Create mytext variable
	char mytext[MAXCHAR];	
	strcpy(mytext, "");
	
	char output[MAXCHAR];
	strcpy(output, "");

	//Open file to cypher
	//int err;
	FILE *fp;
	char str[MAXCHAR];
	//char *strbis[MAXCHAR];
	char *filename = "./Input";

	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("Could not open file %s\n", filename);
		return 1;
	}
	printf("Opening file :\n");

	int count = 0;	
	while(fgets(str, MAXCHAR, fp)!=NULL){ 
		strcat(mytext, str);
		int len = strlen(str);
		count = count + len;
		printf("%d :%s \n", len, str);
	}
	printf("Count is: %d \n", count);
    printf("%s \n", mytext);
    fflush(stdout);
	fclose(fp);
	
	// ----------- Répartition travail -------------
	
	//Calculate optimal load of the processus
	int proc_load = count/PROC;
	if(count%PROC != 0)
	{
		proc_load += 1;
	}
	printf("Char/processus = %d\n", proc_load);

	//Filling text_buffer
	printf("\n--Filling text_buffer:\n");
    int buffer_index = 0;
    int l, k = 0;
    while(buffer_index < PROC)
    {
		l = 0;
		while(l < proc_load && mytext[k] != '\0')
		{
			printf("buffer_index: %d, l:%d, k:%d, ch: %c \n", buffer_index, l, k, mytext[k]);
			text_buffer[buffer_index][l] = mytext[k];
			k++;
			l++;
		}
		text_buffer[buffer_index][l] = '\0';
		printf("<%s>\n", text_buffer[buffer_index]);
		buffer_index++;
    }
	// ------------------ SHM -----------------

	// Initialization of the shared buffer

	// Our memory segment is named "buff"
	// Create the segment
	// Create a str from the text
	char **shm;
	int shmid;
	if((shmid = shmget(IPC_PRIVATE, sizeof(text_buffer), IPC_CREAT | 0666)) < 0 )
	{
		perror("shmget");
		exit(1);
	}
	printf("shmid: %d\n", shmid);
	// Attach the segment to our data space
	shm = shmat(shmid, NULL, 0);
	if((int) shm  == -1)
	{
		perror("shmat error");
		exit(1);
	}

	else printf("Attached\n");
	
	//printf("Contenu de la mémoire : %d\n", shmids);	
	
// ------------------ PROCESSUS ---------------------
	int fils = 0;
	//int d = 0;
	int b = 0;
	
	int flag = 0;
	int status;
	//int test = 0;
	//int j = 0;
	int q =0;

	while(flag != PROC){
		pid_t pid = fork();

		if(pid == 0)
		{
			fils = 1;
			break;
		}

		else
		{
			//Prout.wav
		}
		flag++;
		b++;
	}

	if(fils == 1)
	{
		pid = getpid();
		printf("Valeur incrémentation tableau: %d \nTraite: %s ",b, text_buffer[b]);
		for(int i=0; i < strlen(text_buffer[b]);i++)
		{
			text_buffer[b][i] = encrypt(1, text_buffer[b][i]);
		}
		printf("\nChild: Text_buffer[%d]:%s", b, text_buffer[b]);
		memcpy(&shm + MAXCHAR*b, text_buffer[b], sizeof(text_buffer[b]));
	}

	else
	{
		while(q < PROC)
		{
			int w = wait(&status);
			q++;
		}	
		shmdt (shm);
		shmctl(shmid,IPC_RMID,0);
		printf("--Recomposition:\n");	
		fflush(stdout);	
		int l;
		for(l=0; l<PROC; l++)
		{	
			int test = &shm;
			printf("Parent: shm: %d\n", test);
			//strcat(output, shm[l]);
		}	
		printf("OUTPUT:\n%s", output);
	}

return 0;
}



