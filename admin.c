#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include "structures.h"
#include "common.h"

int addfaculty();
//for checking
int main()
{
	int status=addfaculty();
	if(status)
	{
		printf("successfully added faculty\n");
	}
	else
	{
		printf("error increating faculty\n");
	}
	return 0;
}

int addfaculty()
{
	ssize_t readb;
	char readbuff[1000];
	struct faculty lastf,newf;
	
	
	int facfd=open("faculty.txt", O_RDWR | O_CREAT, 0766);
	if(facfd==-1)
	{
		perror("error in opening faculty data\n");
		return 0;
	}
	if(lseek(facfd,0,SEEK_END)==0)
	{
	newf.id=1;
	}
	else if(lseek(facfd,0,SEEK_END)==-1)
	{
		perror("error in lseek operation\n");
		return 0;
	}
	else
	{
		int offset=lseek(facfd,-sizeof(struct faculty),SEEK_END);
		if(offset==-1)
		{
			perror("error in reaching last record\n");
			return 0;
		}
		readb=read(facfd,&lastf,sizeof(struct faculty));
		if(readb==-1)
		{
			perror("error in getting last record\n");
			return 0;
		}
		else
		{
			newf.id=lastf.id+1;
		}
	}
	printf("%d",newf.id);
	char nid[10];
	sprintf(nid,"%d",newf.id);
	char get_loginid[20];
	 snprintf(get_loginid,sizeof(get_loginid),"Fac_%s",nid);
	 strcpy(newf.loginid,get_loginid);
	char* password=generateRandomPassword();
	strcpy(newf.password,password);
	free(password);
	printf("Enter name: \n");
	ssize_t bytesRead = read(STDIN_FILENO, newf.name, sizeof(newf.name));
	if (bytesRead <= 1){
        perror("Error in storing name\n"); //equal to 1 for handling empty name
        return 0;
    	}
    	printf("Enter email: \n");
	bytesRead = read(STDIN_FILENO, newf.email, sizeof(newf.email));
	if (bytesRead <= 1){
        perror("Error in storing email\n");
        return 0;
    	} 
    	char mob[11];
    	printf("Enter mobile number: \n");
	bytesRead = read(STDIN_FILENO, mob, sizeof(mob));
    	if (bytesRead <= 10){
        perror("Error in storing mobile no.\n");
        return 0;
    	}
    	for(int i=0;i<10;i++)
    	{
    	newf.mobno[i]=mob[i]- '0';
    	}
	for(int i=0;i<20;i++)
	{
	newf.offered_courses[i]=0;
	}
	
	printf("name=%s",newf.name);
    	printf("email=%s",newf.email);
    	printf("mobile=");
    	for(int i=0;i<10;i++)
    	{
    	printf("%d",newf.mobno[i]);
    	}
	printf("\n");
	printf("id=%d\n",newf.id);
	printf("loginid=%s\n",newf.loginid);
	printf("password=%s\n",newf.password);
	ssize_t bytesWrite=write(facfd,&newf,sizeof(struct faculty));
	if (bytesWrite <= 0){
        perror("Error in writing struct\n");
        return 0;
    	}
    	
    	close(facfd);
	return 1;
}

