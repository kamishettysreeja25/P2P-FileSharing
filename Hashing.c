
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
#include "Hashing.h"
void Filehash(char read_buffer[] , int *newsockfd)
{
	char write_buffer[1024];
	bzero(write_buffer,1024);
	char v[6]="Verify",c[8]="CheckAll";
	int t=0,i;
	for(i=0;i<6;i++)
		if(v[i]!=read_buffer[9+i])
			t=1;
	if(t==1)
	{
		for(i=0;i<8;i++)
			if(c[i]!=read_buffer[9+i])
				t=2;
	}
	if(t==0)
	{
		char l1[500],l2[400];
		int k=0;
		i=16;
		while(i!=strlen(read_buffer))
		{
			l1[k]=read_buffer[i];
			k++;
			i++;
		}
		l1[k]='\0';
		hashing(l1,newsockfd);
		bzero(write_buffer,1024);
		strcpy(write_buffer,"END");
		if(write(*newsockfd, write_buffer, 1024) < 0)
		{
			error("\nERROR: Writing to socket");
			exit(1);
		}

	}
	if(t==1)
	{
		system("ls -l |grep -v ^d | awk '{print $9}' | tail -n +2 > fd");
		int flag=0;
		FILE *file = fopen ( "fd", "r" );
		if ( file != NULL )
		{
			char line [500];
			while ( fgets ( line, sizeof(line), file ) != NULL )
			{
				char te[400];
				int k=0;
				i=0;
				while(line[i]!='\n')
				{
					te[k]=line[i];
					k++;
					i++;
				}
				te[k]='\0';
				hashing(te,newsockfd);
			}
			flag=1;
		}
		else
			error("Error in file opening");
		if(flag==1)
		{
		bzero(write_buffer, 1024);
		strcpy(write_buffer,"END");
		if(write(*newsockfd, write_buffer, 1024) < 0)
		{
			error("\nERROR: Writing to socket");
			exit(1);
		}
		}
		system("rm -rf fd");

	}

}
void hashing(char l1[],int *newsockfd)
{
	int i=0,k=0;
	char write_buffer[1024];
		char str[500],st[100];
		strcpy(str,"md5sum ");
		strcat(str,l1);
		strcat(str," > result");
		system("touch result");
		system("touch result1");
		strcpy(st,"stat ");
		strcat(st,l1);
		strcat(st," | grep Modify | awk '{print $3}' > result1");
		system(str);
		system(st);
		FILE *f=fopen("result","r");
		FILE *f1=fopen("result1","r");
		char u[300],r[300],a[300];
		fgets(u,sizeof(u),f);
		fgets(r,sizeof(r),f1);
		system("rm -rf result*");
		i=0,k=0;
		while(u[i]!=' ')
		{
			a[k]=u[i];
			i++;
			k++;
		}
		a[k]='\0';
		strcat(l1,"\t");
		r[strlen(r)-1]='\0';
		strcat(l1,r);
		strcat(l1,"\t");
		strcat(l1,a);
		strcpy(write_buffer,l1);
		if(write(*newsockfd, write_buffer, 1024) < 0)
		{
			error("\nERROR: Writing to socket");
			exit(1);
		}
}
void cFilehash(char write_buffer[], int *sockfd)
{
	int n;
	char read_buffer[1024];
	n = write(*sockfd,write_buffer,1024);
	if (n < 0)
		error("ERROR writing to socket");
	int t=0,i;
	char v[6]="Verify",c[8]="CheckAll";
	for(i=0;i<6;i++)
		if(v[i]!=write_buffer[9+i])
			t=1;
	if(t==1)
	{
		for(i=0;i<8;i++)
			if(c[i]!=write_buffer[9+i])
				t=2;
	}
	if(t==0)
	{

		int f=0,flag=1;
		bzero(read_buffer, 1024);
		printf("\nRecieved data : %s\n",read_buffer);
		printf("File-name	Latest-Timestamp	Md5hash\n");
		while((f= read(*sockfd, read_buffer,1024)) > 0)
		{
			if(strcmp(read_buffer,"END")==0)
			{flag=0;
				break;
			}
			else
				printf("%s\n",read_buffer);
		}
		if(flag==1)
		exit(0);
	}
	if(t==1)
	{
		int f=0,flag=1;
		bzero(read_buffer, 1024);
		printf("\nRecieved data : %s\n",read_buffer);
		printf("File-name	Latest-Timestamp	Md5hash\n");
		while((f= read(*sockfd, read_buffer,1024)) > 0)
		{
			if(strcmp(read_buffer,"END")==0)
			{flag=0;
				break;
			}
			else
				printf("%s\n",read_buffer);
		}
		if(flag==1)
		exit(0);
	}
}
