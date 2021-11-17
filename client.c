#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>

struct sockaddr_in DSS_Addr, DataSub1, DataSub2, DataSub3, ServSub1, ServSub2, ServSub3;
int controlfd, child1, child2, child3, datafd1, datafd2, datafd3;
int control_port = 9082;
int data_port1 = 9083;
int data_port2 = 9084;
int data_port3 = 9085;
char server_IP[20] = "192.168.254.8";
int parent,pid1, pid2, pid3;
int pipefd1[2], pipefd2[2], pipefd3[2];

int main()
{

	// init pipes
	pipe(pipefd1[2]);
	pipe(pipefd2[2]);
	pipe(pipefd3[2]);

	pipefd1[1] = -1;
	pipefd2[1] = -1;
	pipefd3[1] = -1;

	//form control socket 
	if ( (controlfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
		{
        		perror("socket creation failed");
        		exit(EXIT_FAILURE);
    		}

	//form address
	DSS_Addr.sin_family = AF_INET;
	DSS_Addr.sin_port = htons(control_port);
	DSS_Addr.sin_addr.s_addr = inet_addr(server_IP);

	DataSub1.sin_family = AF_INET;
	DataSub1.sin_port = htons(data_port1);
	DataSub1.sin_addr.s_addr = inet_addr(server_IP);

	DataSub2.sin_family = AF_INET;
	DataSub2.sin_port = htons(data_port2);
	DataSub2.sin_addr.s_addr = inet_addr(server_IP);

	DataSub3.sin_family = AF_INET;
	DataSub3.sin_port = htons(data_port3);
	DataSub3.sin_addr.s_addr = inet_addr(server_IP);

	//establish "control" connection
	if (connect(controlfd, (struct sockaddr *) &DSS_Addr, sizeof(DSS_Addr)) != 0) 
	{
		perror("connection with server failed");
		exit(EXIT_FAILURE);	
	}

	//establish and fork 3 connections so each process is a TCP subflow
	
	parent = getpid();
	// ctrl conn


	// make connection to first data subflow 
	if ((connect(datafd, (struct sockaddr *) &DataSub1, sizeof(DataSub1)) != 0)) 
	{
		perror("data connection with client failed");
		exit(EXIT_FAILURE);
	}
	if(fork() == 0)
	{
		pid1 = getpid();
	}	
	else
	{	// make connection to second data subflow 
		if ((connect(datafd, (struct sockaddr *) &DataSub2, sizeof(DataSub2)) != 0)) 
		{
			perror("data connection with client failed");
			exit(EXIT_FAILURE);
		} 
		if(fork() == 0)
		{
			pid2 = getpid();
		}	
		else
		{	// make connection to third data subflow
			if ((connect(datafd, (struct sockaddr *) &DataSub3, sizeof(DataSub3)) != 0)) 
			{
				perror("data connection with client failed");
				exit(EXIT_FAILURE);
			}
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
			break;
		}
		else if(currentProcess == pid1)
		{
	

			if(pipefd1[0] != -1)
			{
				printf("c1 %d\n", pipefd1[0]);
				break;
			}
			
		} 
		else if(currentProcess == pid2)
		{			
			if(pipefd2[0] != -1)
			{
				printf("c2 %d\n", pipefd2[0]);
				break;
			}

		} 
		else if(currentProcess == pid3)
		{

			if(pipefd3[0] != -1)
			{
				printf("c3 %d\n", pipefd3[0]);
				break;
			}
		} 

	}
	
	return 0;
}
