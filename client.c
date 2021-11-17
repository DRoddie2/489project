#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <pthread.h>


int main()
{
	int parent,pid1, pid2, pid3;
	int pipefd1[2], pipefd2[2], pipefd3[2];

	// init pipes
	pipe(pipefd1[2]);
	pipe(pipefd2[2]);
	pipe(pipefd3[2]);

	pipefd1[1] = -1;
	pipefd2[1] = -1;
	pipefd3[1] = -1;

	parent = getpid();
	// ctrl conn


	// make conn1 
	if(fork() == 0)
	{
		pid1 = getpid();
	}	
	else
	{	// make conn2 
		if(fork() == 0)
		{
			pid2 = getpid();
		}	
		else
		{	// make conn3 
			if(fork() == 0)
			{
				pid3 = getpid();
			}	
		}
	}
	
	for(;;)
	{
		int currentProcess = getpid();
		if(currentProcess == parent)
		{
			//printf("parent %d\n", currentProcess);
			pipefd1[1] = 56;
			pipefd2[1] = 46;
			pipefd3[1] = 36;
		}
		else if(currentProcess == pid1)
		{
	

			if(pipefd1[0] != -1)
			{
				printf("c1 %d\n", pipefd1[0]);
			}
			
		} 
		else if(currentProcess == pid2)
		{			
			if(pipefd2[0] != -1)
			{
				printf("c2 %d\n", pipefd2[0]);
			}

		} 
		else if(currentProcess == pid3)
		{

			if(pipefd3[0] != -1)
			{
				printf("c3 %d\n", pipefd3[0]);
			}
		} 
		//break;
	}
	
	return 0;
}
