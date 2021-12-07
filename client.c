#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "util.h"

#define SERVER_IP "127.0.0.1"

struct sockaddr_in DSS_Addr, DataSub1, DataSub2, DataSub3, ServSub1, ServSub2, ServSub3;
int controlfd, subflow1, subflow2, subflow3, datafd1, datafd2, datafd3;
int parent, pid1, pid2, pid3;
int cp1[2], cp2[2], cp3[2];
char bytes[992];

/* Helper fun'n for generating the */
void generateBytes()
{
	char subMsg[62] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int i, j;
	for(i = 0; i < 16; i++)
	{
		int base = i * 62;
		for(j = 0; j < 62; j++)
		{
			bytes[base + j] = subMsg[j];
		}
	} 
}

int main()
{
	/* init variables */
	Set logData[248];
	int subSeqNum1, subSeqNum2, subSeqNum3; // updated by child processes
	int sub1, sub2, sub3, last1, last2, last3; // used by parent process to track subsequence #

	subSeqNum1 = 0; // counting starts at 0
	subSeqNum2 = 0;
	subSeqNum3 = 0;
	last1 = 0;
	last2 = 0;
	last3 = 0;

	parent = getpid(); 
	generateBytes(); // fill bytes array

	/* initalize pipes for communication b/w parent and children */
	initPipes(cp1, cp2, cp3);
	write(cp1, "0", MSGSIZE);
	write(cp2, "0", MSGSIZE);
	write(cp3, "0", MSGSIZE);

	/* initalize sockets for the control and 3 subflow connections */
	initSocket(&controlfd);
	initSocket(&subflow1);
	initSocket(&subflow2);
	initSocket(&subflow3);

	/* form addresses for the control and 3 subflow connections */
	formClientAddress(&DSS_Addr, SERVER_IP, control_port);
	formClientAddress(&DataSub1, SERVER_IP, data_port1);
	formClientAddress(&DataSub2, SERVER_IP, data_port2);
	formClientAddress(&DataSub3, SERVER_IP, data_port3);

	/* connect the control and 3 subflow sockets to the server*/
	connectSocket(&controlfd, &DSS_Addr, sizeof(DSS_Addr), "ctrl conn failed");
	connectSocket(&subflow1, &DataSub1, sizeof(DataSub1), "subflow1 conn failed");
	connectSocket(&subflow2, &DataSub2, sizeof(DataSub2), "subflow2 conn failed");
	connectSocket(&subflow3, &DataSub3, sizeof(DataSub3), "subflow3 conn failed");

	/* fork child processes 1,2,3 and store process id numbers */
	if(fork() == 0)
	{
		pid1 = getpid();
	}	
	else
	{	
		if(fork() == 0)
		{
			pid2 = getpid();
		}	
		else
		{	
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
			char recStr[1000];
			char sendStr[MSGSIZE];

			read(cp1[0], recStr, 1000);
			sub1 = atoi(recStr);
			while((sub1 - last1) > 0)
			{
				int mappedSeq = last1*3;
				sprintf(sendStr, "%d", mappedSeq);
				write(controlfd, sendStr, MSGSIZE);
				
				last1++;
				
			} 
			

			read(cp2[0], recStr, MSGSIZE);
			sub2 = atoi(recStr);
			while((sub2 - last2) > 0)
			{
				int mappedSeq = last2*3 + 1;
				sprintf(sendStr, "%d", mappedSeq);
				write(controlfd, sendStr, MSGSIZE); 
				
				sleep(0.1);
				last2++;
				
			} 

			read(cp3[0], recStr, MSGSIZE);
			sub3 = atoi(recStr);
			while((sub3 - last3) > 0)
			{
				int mappedSeq = last3*3 + 2;
				sprintf(sendStr, "%d", mappedSeq);
				write(controlfd, sendStr, MSGSIZE);
			
				sleep(0.1);
				last3++;
				
			} 
			if(last1 >= 83 && last2 >= 83 && last3 >= 82) break;				
			
		}
		else if(currentProcess == pid1)
		{
			// build msg
			if(subSeqNum1 >= 83) break;	
			char msg[MSGSIZE];
			int idx = subSeqNum1 * 12;
			sprintf(msg, "%c%c%c%c\0", bytes[idx], bytes[idx+1], bytes[idx+2], bytes[idx+3]);
		
			// send msg
			if(write(subflow1, msg, 1000) < 0)
			{
				perror("read failed");
				exit(1);
			}


			// tell parent msg was sent & update seq number
			subSeqNum1++;
			char cpStr[MSGSIZE];
			sprintf(cpStr, "%d", subSeqNum1);
			write(cp1[1], cpStr, MSGSIZE);		
		} 
		else if(currentProcess == pid2)
		{
			if(subSeqNum2 >= 83) break;	
	
			// build msg
			char msg[MSGSIZE];
			int idx = subSeqNum2 * 12 + 4;
			sprintf(msg, "%c%c%c%c\0", bytes[idx], bytes[idx+1], bytes[idx+2], bytes[idx+3]);

			// send msg
			if(write(subflow2, msg, 1000) < 0)
			{
				perror("read failed");
				exit(1);
			}

			// tell parent msg was sent & update seq number
			subSeqNum2++;
			char cpStr[MSGSIZE];
			sprintf(cpStr, "%d", subSeqNum2);
			write(cp2[1], cpStr, MSGSIZE);
			
		} 
		else if(currentProcess == pid3)
		{
			// build msg
			if(subSeqNum3 >= 82) break;	
			char msg[MSGSIZE];
			int idx = subSeqNum3 * 12 + 8;
			sprintf(msg, "%c%c%c%c\0", bytes[idx], bytes[idx+1], bytes[idx+2], bytes[idx+3]);
			
			// send msg
			if(write(subflow3, msg, 1000) < 0)
			{
				perror("read failed");
				exit(1);
			}

			// tell parent msg was sent & update seq number
			subSeqNum3++;
			char cpStr[MSGSIZE];
			sprintf(cpStr, "%d", subSeqNum3);
			write(cp3[1], cpStr, MSGSIZE);
		} 
		
	}

	/* print final buffer and log */
	if(getpid() == parent)
	{
 		printf("\nMessage Sent\n\n%s\n\n", bytes);
		printf("Client Side - Sequence Number Log\n\n");
		printLog(logData, 248);
	}

	close(controlfd);
	close(datafd1);
	close(datafd2);
	close(datafd3);
	return 0;
}
