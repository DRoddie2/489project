#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <pthread.h>


int main()
{
	int conn1, conn2, conn3;
	pid_t parent, id1, id2, id3;
	
	parent = getpid();
	// ctrl conn


	// make conn1 
	id1 = fork();
	if(id1 == 0)
	{
		conn1 = getpid();
	}	
	else
	{	// make conn2 
		id2 = fork();
		if(id2 == 0)
		{
			conn2 = getpid();
		}	
		else
		{	// make conn3 
			id3 = fork();
			if(id3 == 0)
			{
				conn3 = getpid();
			}	
		}
	}
	
	for(;;)
	{
		int currentProcess = getpid();
		if(currentProcess == parent)
		{
			printf("parent %d\n", currentProcess);
		}
		else if(currentProcess == conn1){printf("c1 %d\n", currentProcess);} 
		else if(currentProcess == conn2){printf("c2 %d\n", currentProcess);} 
		else if(currentProcess == conn3){printf("c3 %d\n", currentProcess);} 
		break;
	}
	
	return 0;
}
