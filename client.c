#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <pthread.h>

<<<<<<< HEAD

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

=======
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
	
>>>>>>> c14f4644e16c8318e89fa6679b225d40c87a59d3
	parent = getpid();
	// ctrl conn


	// make conn1 
<<<<<<< HEAD
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
=======
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
>>>>>>> c14f4644e16c8318e89fa6679b225d40c87a59d3
			}	
		}
	}
	
	for(;;)
	{
		int currentProcess = getpid();
		if(currentProcess == parent)
		{
<<<<<<< HEAD
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
=======
			printf("parent %d\n", currentProcess);
		}
		else if(currentProcess == conn1){printf("c1 %d\n", currentProcess);} 
		else if(currentProcess == conn2){printf("c2 %d\n", currentProcess);} 
		else if(currentProcess == conn3){printf("c3 %d\n", currentProcess);} 
		break;
>>>>>>> c14f4644e16c8318e89fa6679b225d40c87a59d3
	}
	
	return 0;
}
