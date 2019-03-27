
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 16384
#define  BUFSIZE  10
#define text_size 0
#define PROC 4


pid_t fork(void);
pid_t wait(int* status);

int main()
{
	//Create a str from the text
	char *shm;
	
	//Create mytext variable
	char mytext[MAXCHAR];
	char text [MAXCHAR];	
	
	strcpy(mytext, "");
	char output[MAXCHAR];

	//Open file to cypher
	int k,l,count = 0;
	int err;
	FILE *fp, *fl ;
	char str[MAXCHAR];
	char* filename = "./Input";

	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("Could not open file %s\n", filename);
		return 1;
		}

	printf("--Opening file :\n");
	/*
	
	//Concatenate text to a single string
	while(fgets(str, MAXCHAR, fp) != NULL){
		strcat(mytext,str);
		int len = strlen(str);
		printf("%d :%s \n", len, str);
		count = count + len;
		}

	printf("String lenght is : %d\n", count);
	printf("%s\n", mytext);
	fclose(fp);

	*/
	// Put the string in an array
	int i=0;
	char* string[100];
	//char line[100];
 
	fl = fopen(filename, "r"); 
 
	while(fgets(str, MAXCHAR, fl)!=NULL){ 
		printf("%s",str);
		string[i] = str;
		i++;
	}
 
	for (i=0 ; i<6; i++) {
	
		printf("array ; %s\n", string[i]);
 
	}
	fclose(fl);

	
	//Calculate optimal load of the processus
	int proc_load = count/PROC;
	
	if(count%PROC != 0){
		proc_load += 1;
		}

	printf("Char/processus = %d\n", proc_load);

/*
	char str_1[proc_load];
	int len = strlen(str);
	
	fgets(str_1, proc_load, fp)
	*/
	
	
	/*while(fgets(str, MAXCHAR, fp) != NULL){
		strcat(mytext,str);
		int len = strlen(str);
		printf("%d :%s \n", len, str);
		count = count + len;
		}
	
*/






	//Initialization of the shared buffer

	//Our memory segment is named "buff"
	//Create the segment
	int shmid;
	if((shmid = shmget(IPC_PRIVATE, MAXCHAR, IPC_CREAT | 0666)) < 0 ){
		perror("shmget");
		exit(1);
		}

	//Attach the segment to our data space
	shm = (char *) shmat(shmid, NULL, 0);
	if((int) shm  == -1){
		perror("shmat error");
		exit(1);
		}

	else printf("Attached\n");
	
	
	shm = *string;
	
	//Put our string into the memory for thesson processus to access
	//printf("contenu de mytext: %s\n", mytext);
	//printf("contenu de my: %c\n", mytext[5]);
	//*shm = mytext;

	//test
	printf("Contenu de la mémoire : %s\n", shm );
    //typeof(*shm);

//	int fils = 0;
//	int i = 0;
//	int  buffer[BUFSIZE];
//	int flag =0;
//	int status;
//	int test = 0;
//	printf(" i :%i \n",i);
//	int j = 0;
//	while( j < BUFSIZE) {

//		buffer[j] = 0 ;
//		j++ ;

/*		}
	while(flag != PROC){
		pid_t pid = fork();

		if ( pid == 0 ){
			fils = 1;
			break;
			}

		else {
			i++;
			}

		flag++;
		}




	if(fils == 1){

//		printf("Code exécuté par fils\n");
		buffer[1] = 19;
		printf("buffer 1 : '%d'", buffer[1]);
		}

	else{
		int truc =0;
		while(truc < 2){
			int w = wait(&status);
			printf("wait a fini avec %d\n", w);
			printf(" valeur du buffer dans père : %d", buffer[1]);
		//	test += buffer[i];
			truc = (truc+1) % BUFSIZE;
			}
		printf(" valeur finale : %d", buffer[1]);
		printf(" test : %d", test);
		}

	printf("%i\n", test);
//	printf(" %i\n",getpid());
//	buffer[i]  = getpid();
//	printf("buff : %i\n",buffer[i]);

//	i = (i + 1) % BUFSIZE;



*/
return 0;
}




