#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct sockaddr_in DSS_ServAddr;
int controlfd;
int control_port = 9082;

int main(int argc, char** argv)
{
	//form initial socket 
	if ( (controlfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) 
	{
        	perror("control socket creation failed");
        	exit(EXIT_FAILURE);
    	}

	//form server address
	DSS_ServAddr.sin_family = AF_INET;
	DSS_ServAddr.sin_port = htons(control_port);
	DSS_ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( bind(controlfd, (const struct sockaddr *) &DSS_ServAddr, sizeof(DSS_ServAddr)) < 0 )
    	{
      		perror("initial bind failed");
        	exit(EXIT_FAILURE);
    	}

	if ((listen(controlfd, 5)) != 0) 
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);	
	}
	
	// call accept()
}
