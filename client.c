#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <pthread.h>

struct sockaddr_in DSS_Addr, DataSub1, DataSub2, DataSub3;
int controlfd, child1, child2, child3;
int control_port = 9082;

int main()
{
	//form control socket 
	if ( (controlfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
		{
        		perror("socket creation failed");
        		exit(EXIT_FAILURE);
    		}

	//form address
	DSS_Addr.sin_family = AF_INET;
	DSS_Addr.sin_port = htons(control_port);
	DSS_Addr.sin_addr.s_addr = inet_addr(serv_IP);

	//establish "control" connection
	if (connect(controlfd, (struct sockaddr *) &DSS_Addr, sizeof(DSS_Addr)) != 0) 
	{
		perror("connection with server failed");
		exit(EXIT_FAILURE);	
	}

	//establish and fork 3 connections so each process is a TCP subflow

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
