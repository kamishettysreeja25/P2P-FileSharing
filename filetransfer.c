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
#include "filetransfer.h"

void cFiledownload(char write_buffer[],int *sockfd)
{
	int n=0;
	char read_buffer[1024],hashm[200],data[1024];
    	n = write(*sockfd,write_buffer, 1024);
    		if (n < 0)
        		 error("ERROR writing to socket");
	int i=0,k=0;
	char xname[200],timestamp[200],size[100],name[200];
	printf("[Client] File recieving from server\n");
	while(write_buffer[13+i]!='\0')
	{
		name[i]=write_buffer[13+i];
		i++;
	}
	name[i]='\0';
	bzero(read_buffer,1024);
	if(n= read(*sockfd, read_buffer,1024) > 0)
	{
		if(strncmp(read_buffer,"File Not there",14)==0)
		{
			return;
		}

	}
	else
		error("Error in reading from socket");

	bzero(read_buffer,1024);
	if(n= read(*sockfd, read_buffer,1024) > 0)
	{

			if(strncmp(read_buffer,"timestamp",9)==0)
			{
				i=9;
				k=0;
				while(read_buffer[i]!='\0')
				{
					timestamp[k]=read_buffer[i];
					k++;
					i++;
				}
				timestamp[k]='\0';
			}
	}
	
	else
		error("Error in reading from socket");
	bzero(read_buffer,1024);
	if(n= read(*sockfd, read_buffer,1024) > 0)
	{
			if(strncmp(read_buffer,"size",4)==0)
			{
				i=4;
				k=0;
				while(read_buffer[i]!='\0')
				{
					size[k]=read_buffer[i];
					k++;
					i++;
				}
				size[k]='\0';

			}
	}
	else
		error("Error in reading from socket");
	bzero(read_buffer,1024);
	if(n= read(*sockfd, read_buffer,1024) > 0)
	{
			if(strncmp(read_buffer,"name",4)==0)
			{
				i=4;
				k=0;
				while(read_buffer[i]!='\0')
				{
					xname[k]=read_buffer[i];
					k++;
					i++;
				}
				xname[k]='\0';

			}
	}
	else
		error("Error in reading from socket");
	bzero(read_buffer,1024);
	if(n= read(*sockfd, read_buffer,1024) > 0)
	{
			if(strncmp(read_buffer,"hash",4)==0)
			{
				i=4;
				k=0;
				while(read_buffer[i]!='\0')
				{
					hashm[k]=read_buffer[i];
					k++;
					i++;
				}
				hashm[k]='\0';

			}
	}
	else
		error("Error in reading from socket");
	printf("Timestamp	%s\n",timestamp);
	printf("Size		%s\n",size);
	printf("Name		%s\n",name);
	printf("Hash		%s\n",hashm);
	bzero(read_buffer,1024);
	FILE *fr = fopen(name, "wb");
	if(fr == NULL)
		printf("File %s Cannot be opened file on peer.\n", name);
	else
	{
		int f = 0,f1=0;
		unsigned int size = 0;
		while(f = recv(*sockfd,&size,sizeof(size),0)>0)
		{
			int p=ntohl(size);
			f1 = recv(*sockfd, read_buffer,1024, 0);
			if(strcmp(read_buffer,"END")==0)
				break;
			if(f1 > 0) {

				int w= fwrite(read_buffer, sizeof(char), p, fr);
				if(w < p)
				{
					error("[Client] File write failed on client.\n");
				}
				if (p == 0 || p!= 1024)
				{
					break;
				}
			}
			if(f1 < 0)
			{
				error("Error receiving file from client to server.\n");
			}
			if(f < 0)
			{
				error("Error receiving file from client to server.\n");
			}
			bzero(read_buffer, 1024);
		}
		printf("[Client]File received from server!\n");
		fclose(fr);
	}
}

void Filedownload(char read_buffer[],int *newsockfd)
{

	char write_buffer[1024];
	bzero(write_buffer,1024);
	int t=0,i=0,n=0;
	char name[200],com[500];
	while(read_buffer[13+i]!='\0')
	{
		name[i]=read_buffer[13+i];
		i++;
	}
	name[i]='\0';
	bzero(write_buffer,1024);
	FILE *f = fopen(name,"r");
	if(f==NULL)
	{
		strcpy(write_buffer,"File Not there");
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
		return;
	}
	else
	{
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
		fclose(f);
	}
	bzero(write_buffer,1024);
	strcpy(write_buffer,"timestamp");
		system("touch r.txt");
		strcpy(com,"stat ");
		strcat(com,name);
		strcat(com," | grep Modify | awk '{print $3}' > r.txt");
	system(com);
	f=fopen("r.txt","r");
	char u[300],a[300];
	fgets(u,sizeof(u),f);
	strcat(write_buffer,u);
	fclose(f);
	system("rm -rf r.txt");
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
	bzero(write_buffer,1024);
	strcpy(write_buffer,"size");
	strcpy(com,"stat ");
	strcat(com,name);
	strcat(com," | grep Size | awk '{print $2}' > r.txt");
	system(com);
	f=fopen("r.txt","r");
	fgets(u,sizeof(u),f);
	fclose(f);
	strcat(write_buffer,u);
	system("rm -rf r.txt");
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
	bzero(write_buffer,1024);
	strcpy(write_buffer,"name");
	strcat(write_buffer,name);
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
	char str[500];
	strcpy(str,"md5sum ");
	strcat(str,name);
	strcat(str," > r.txt");
	system(str);
	f=fopen("r.txt","r");
	fgets(u,sizeof(u),f);
	int k=0;
	i=0,k=0;
	while(u[i]!=' ')
	{
		a[k]=u[i];
		i++;
		k++;
	}
	a[k]='\0';
	strcpy(write_buffer,"hash");
	strcat(write_buffer,a);
	if(n=write(*newsockfd,write_buffer,1024)<0)
	{
		error("Error in writing in socket");
	}
	system("rm -rf r.txt");
	FILE *fs = fopen(name, "rb");
	if(fs == NULL)
	{
		printf("ERROR: File %s not found on server.\n", name);
		exit(1);
	}

	bzero(write_buffer, 1024);
	int f1;
	while((f1 = fread(write_buffer, sizeof(char), 1024, fs))>0)
	{
		unsigned int sdlen=htonl(f1);
		if(send(*newsockfd,&sdlen,sizeof(sdlen),0) < 0){
			error("[Server] Sending Header Failed : ");
			break;
		}
		if(send(*newsockfd, write_buffer, f1,0) < 0)
		{
			printf("ERROR: Failed to send file %s.\n", name);
			exit(1);
		}
		bzero(write_buffer, 1024);
	}
	bzero(write_buffer, 1024);
	strcpy(write_buffer,"END");
	if(send(*newsockfd, write_buffer, 1024,0) < 0)
	{
		printf("ERROR: Failed to write in socket \n");
		exit(1);
	}
	fclose(fs);
	printf("[Server]File sent to client!\n");
}
void cFileupload(char write_buffer[],int *sockfd)
{
	int n=0;
	char read_buffer[1024];
    	n = write(*sockfd,write_buffer, 1024);
    		if (n < 0)
        		 error("ERROR writing to socket");
	int i=0,k=0;
	char size[100],name[200],u[300],com[400];
	while(write_buffer[11+i]!='\0')
	{
		name[i]=write_buffer[11+i];
		i++;
	}
	name[i]='\0';
	bzero(write_buffer,1024);
	strcpy(write_buffer,"name");
	strcat(write_buffer,name);
	if(n= write(*sockfd, write_buffer,1024) < 0)
	{
		error("Error in writing in socket");
	}
	bzero(write_buffer,1024);
	strcpy(write_buffer,"size");
	strcpy(com,"stat ");
	strcat(com,name);
	strcat(com," | grep Size | awk '{print $2}' > r.txt");
	system(com);
	FILE *f;
	f=fopen("r.txt","r");
	fgets(u,sizeof(u),f);
	fclose(f);
	strcat(write_buffer,u);
	system("rm -rf r.txt");
	if(n= write(*sockfd, write_buffer,1024) < 0)
	{
		error("Error in writing in socket");
	}

	bzero(read_buffer,1024);
	if(n=read(*sockfd,read_buffer,1024)<0)
	{
		error("Error in reading from socket");
	}
	if(strcmp(read_buffer,"FileUpload Deny")==0)
	{
		printf("[Client] Server denied to upload\n");
		return ;
	}
	if(strcmp(read_buffer,"FileUpload Allow")==0)
	{
		printf("%s\n",name);
		printf("[Client ]File uploading to server\n");
		FILE *fs = fopen(name, "rb");
		if(fs == NULL)
		{
			printf("ERROR: File %s not found on server.\n", name);
			exit(1);
		}

		bzero(write_buffer, 1024);
		int f1;
		while((f1 = fread(write_buffer, sizeof(char), 1024, fs))>0)
		{
			unsigned int sdlen=htonl(f1);
			if(send(*sockfd,&sdlen,sizeof(sdlen),0) < 0){
				error("[Server] Sending Header Failed : ");
				break;
			}
			if(send(*sockfd, write_buffer, f1,0) < 0)
			{
				printf("ERROR: Failed to send file %s.\n", name);
				exit(1);
			}
			bzero(write_buffer, 1024);
		}
		bzero(write_buffer, 1024);
		strcpy(write_buffer,"END");
		if(send(*sockfd, write_buffer, 1024,0) < 0)
		{
			printf("ERROR: Failed to write in socket \n");
			exit(1);
		}
		fclose(fs);

		printf("[Client] File uploaded to the server\n");
	}

}
void Fileupload(char read_buffer[],int *newsockfd)
{
	int n=0;
	int i=0,k=0;
	char size[300],name[300],write_buffer[1024];
	printf("[Server] File recieving from client\n");
	if(n= read(*newsockfd, read_buffer,1024) > 0)
	{
		if(strncmp(read_buffer,"name",4)==0)
		{
			i=4;
			k=0;
			while(read_buffer[i]!='\0')
			{
			name[k]=read_buffer[i];
			i++;k++;
			}
			name[k]='\0';
		}

	}
	else
		error("Error in reading from socket");
	bzero(read_buffer,1024);
	if(n= read(*newsockfd, read_buffer,1024) > 0)
	{
			if(strncmp(read_buffer,"size",4)==0)
			{
				i=4;
				k=0;
				while(read_buffer[i]!='\0')
				{
					size[k]=read_buffer[i];
					k++;
					i++;
				}
				size[k]='\0';
			}
	}
	else
		error("Error in reading from socket");
	printf("[Server] File name	%s\n",name);
	printf("[Server] File size	%s\n",size);
	printf("[Server] Allow/Deny the client to upload: ");
	char p[100];
	kill (pid,SIGSTOP);
	gets(p);
	kill (pid,SIGCONT);
	if(strcmp(p,"Deny")==0)
	{
		bzero(write_buffer,1024);
		strcpy(write_buffer,"FileUpload Deny\0");
		if(n=write(*newsockfd,write_buffer,1024)<0)
		{
        		 error("ERROR writing to socket");
		}
		return;
	}
	else
	{
		bzero(write_buffer,1024);
		strcpy(write_buffer,"FileUpload Allow\0");
		if(n=write(*newsockfd,write_buffer,1024)<0)
		{
        		 error("ERROR writing to socket");
		}
		printf("[Server] Getting file from client\n");
		FILE *fr = fopen(name, "wb");
		if(fr == NULL)
			printf("File %s Cannot be opened file on peer.\n", name);
		else
		{
				bzero(read_buffer, 1024);
			int f = 0,f1=0;
			unsigned int size = 0;
			while(f = recv(*newsockfd,&size,sizeof(size),0)>0)
			{
				int p=ntohl(size);
				f1 = recv(*newsockfd, read_buffer,1024, 0);
				if(strcmp(read_buffer,"END")==0)
					break;
				if(f1 > 0) {

					int w= fwrite(read_buffer, sizeof(char), p, fr);
					if(w < p)
					{
						error("[Server] File write failed on client.\n");
					}
					if (p == 0 || p!= 1024)
					{
						break;
					}
				}
				if(f1 < 0)
				{
					error("Error receiving file from client to server.\n");
				}
				if(f < 0)
				{
					error("Error receiving file from client to server.\n");
				}
				bzero(read_buffer, 1024);
			}
			fclose(fr);
			printf("[Server] File got from client\n");
		}
	}
}