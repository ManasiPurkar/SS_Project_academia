#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "common.h"


bool addfaculty()
{
	ssize_t readb;
	char readbuff[1000];
	struct faculty lastf,newf;
	int facfd=open("faculty.txt", O_RDWR | O_CREAT, 0766);
	if(facfd==-1)
	{
		perror("error in opening faculty data\n");
		return false;
	}
	int offset=lseek(facfd,-sizeof(struct faculty),SEEK_END);
	if(offset==-1)
	{
		perror("error in reaching last record\n");
		return false;
	}
	readb=read(facfd,&lastf,sizeof(struct faculty));
	if(readb==-1)
	{
		perror("error in getting last record\n");
		return false;
	}
	close(facfd);
	if(readb==0)
	{
		newf.id=1;
	}
	else
	{
		newf.id=lastf.id+1;
	}
	char nid[10];
	sprintf(nid,"%d",newf.id);
	newf.loginid=strcat("Fac_",nid);
	char* password=generateRandomPassword();
	strcpy(newf.password,gpassword);
	free(password);
	printf("Enter name:");
	ssize_t bytesRead = read(STDIN_FILENO, newf.name, sizeof(newf.name));
	// Check if reading was successful and add a null terminator
    	if (bytesRead > 0) {
        	newf.name[bytesRead - 1] = '\0'; // Remove the newline character
    	}
    	else {
        perror("Error in storing name\n");
        return false;
    	}
    	printf("Enter email:");
	ssize_t bytesRead = read(STDIN_FILENO, newf.email, sizeof(newf.email));
	// Check if reading was successful and add a null terminator
    	if (bytesRead > 0) {
        	newf.email[bytesRead - 1] = '\0'; // Remove the newline character
    	}
    	else {
        perror("Error in storing email\n");
        return false;
    	}
    	printf("Enter mobile number:");
	ssize_t bytesRead = read(STDIN_FILENO, newf.mobno, sizeof(newf.mobno));
	
    	if (bytesRead <= 0){
        perror("Error in storing mobile no.\n");
        return false;
    	}
	for(int i=0;i<20;i++)
	{
	offered_courses[i]=0;
	}
	return true;
}
