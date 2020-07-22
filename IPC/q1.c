#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<signal.h> 
int _pipe[2];  
pid_t c_id; 

int hs_parent() { //handle siganl parent
    int n =0;
	read(_pipe[0], &n, sizeof(int));
    printf("%d\n",n);
    if (n <5){
        n ++;
        write(_pipe[1], &n, sizeof(int));
        kill(c_id, SIGUSR1);
    }
}
int hs_child() { //handle siganl child
    int n = 0;
    pid_t p_id;
	read(_pipe[0], &n, sizeof(int));
    printf("%d\n",n);
    if (n < 5){
        n++;
        write(_pipe[1], &n, sizeof(int));
        p_id =getppid();
        kill(p_id, SIGUSR1);
    }
    else{
        printf("Son is terminating ...\n");
        exit(0);
    }
}

int main() 
{ 
	if (pipe(_pipe)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	c_id = fork(); 
	if (c_id < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 
	else if (c_id > 0) 
	{ 
        signal(SIGUSR1, hs_parent);
        wait(NULL); //wait for child process.
        printf("Father is terminating ... \n");
        // printf("Father is terminating ... \nSon is terminating ...\n");
        close(_pipe[0]);
        close(_pipe[1]);
	} 
	else
	{ 
        signal(SIGUSR1, hs_child);
        int n=0;
        pid_t p_id;
        write(_pipe[1], &n, sizeof(int));
        p_id =getppid();
        kill(p_id, SIGUSR1);
        while (1)
        {
            
        }

	} 
}