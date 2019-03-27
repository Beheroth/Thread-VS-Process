#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
 
 
int main(void) {
	int i=0;
	char *string[1000];
	char line[1000];
 
	FILE *file;
	file = fopen("Input", "r");

	//create a string array
	while(fgets(line, sizeof line, file)!=NULL){
		printf("line est %s et i est %d\n", line, i);
		string[i]=line;
		printf(" string est : %s\n",string[i]);
		i++;
	}
	fclose(file);
	
	for (i=0 ; i<5; i++) {
		printf("ligne %d est %s\n",i, string[i]);
	}

	char* test[3];
	char ra[1000];
	ra[1] = 'a';
	ra[0] = 'b';
	test[0]=ra;
	printf("test : %s\n", test[0]);


	return 0;
}

