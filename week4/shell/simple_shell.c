#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <malloc.h>

#define MAX_LEN 100 /* The maximum length command */

int main(void) {
  char *args[MAX_LEN / 2 + 1]; /* command line arguments */
  int should_run = 1;          /* flag to determine when to exit program */
  int background = 0;

  while (should_run) {
    printf("my_shell> ");
    fflush(stdout);
    /**
     * 표준입출력으로부터 문자열을 입력 받은 후:
     * (1) fork()를 통해 자식 프로세스를 생성
     * (2) 자식 프로세스가 execvp()를 호출하도록 할 것
     * (3) 만약 입력받은 문자에 &가 포함되어 있으면,
     *     부모 프로세스는 wait() 호출하지 않음
     */
	// input
	char *input = (char*)malloc(MAX_LEN*sizeof(char));
	memset(args, 0, MAX_LEN/2 + 1);
	//char *args = malloc(sizeof(char)MAX_LEN / 2 + 1); /* command line arguments */
	fgets(input, MAX_LEN, stdin);
	if(input == "exit" || input == NULL){
		goto no_input;
	}

	int i=0;
	args[i] = strtok(input, " \n");
	while(args[i] != NULL){
		i ++;
		args[i] = strtok(NULL, " \n");
	}
	
	// on/off background
	char *arg = malloc((i)*sizeof(char*));
	if(strcmp(args[i-1], "&")==0){
		args[i-1] = NULL;
		background = 1;
	}

	// make child process
	pid_t pid = fork();
	if(pid < 0){
		perror("Fork error");
		exit(0);
	}
	// child process's job

	if(pid==0){
		execvp(args[0], args);
	}
	
	// parent process's job	
	int status;
	if(pid > 0){
		if(!background){
			printf("wailting for child, not a background process\n");
			waitpid(pid, &status, 0);
			printf("child process complete\n");
		} else {
			printf("background process\n");
			background = 0;
		}
	}
	no_input:;
  }
  return 0;
}
