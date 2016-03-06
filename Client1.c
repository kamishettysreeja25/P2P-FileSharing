#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "IndexGet.h"
#include "Hashing.h"
#include "manage.h"
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>


pid_t pid;

int main(int argc, char *argv[])
{
	int no;
	if(argc!=2)
	{
		printf("Invalid No of Arguments");
		exit(0);
	}
	printf("Press 0 for TCP and 1 for UDP:");
	scanf("%d",&no);
	
	printf("%d\n",no );
	if(no==0)
	{	
		pid=fork();
		if(pid==0)
		{
			if(no==0)
				manage_peer(argv[1]);
			else if(no==1)
				client(argv[1],argv[2]);
	
		}
		else if(pid)
		{
	
			if(no==0)
				manage_host();
			else if(no==1)
				server(argv[2]);
		}
	}

	else if(no==1)
	{
		server("Udp_file");
		//client(argv[1],argv[2]);
	
	}
	return 0;
}
