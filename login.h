#include<stdio.h>
#include<fcntl.h>
#include <string.h>
#include <unistd.h>
#include<stdlib.h>
#include "structures.h"

int login();
int checkcredentials();
int checkfaculty(char* loginid,char* password);
int checkstudent(char* loginid,char* password);
void logout();


struct user u;
int success=0;


int login(int desc)
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
	strcpy(writeBuffer,"Welcome to login window...\n");
	strcat(writeBuffer,"Available users are 1.Admin(A)  2.Faculty(F)   3.Student(S)\n");
  	strcat(writeBuffer,"Enter user type char: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing options to client!");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading client's choice for ADMIN_MENU");
                return 0;
        }
	
	u.usertype=readBuffer[0];
	bzero(writeBuffer, sizeof(writeBuffer));
	if (u.usertype != 'A' && u.usertype != 'F' && u.usertype != 'S') {
        strcpy(writeBuffer,"Invalid user type. Please enter A, F, or S.\n");
        writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        return 0;
   	}
   	
   	bzero(writeBuffer, sizeof(writeBuffer));
    	strcpy(writeBuffer,"Enter Login id: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing options to client!");
                return 0;
        }
    	bzero(readBuffer, sizeof(readBuffer)); 	
    	readBytes = read(desc, readBuffer, sizeof(readBuffer));
    	strcpy(u.loginid,readBuffer);
    	u.loginid[readBytes-1] = '\0';
    	bzero(writeBuffer, sizeof(writeBuffer));
    	strcpy(writeBuffer,"Enter password: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	bzero(readBuffer, sizeof(readBuffer));
    	readBytes = read(desc, readBuffer, sizeof(readBuffer));
    	strcpy(u.password,readBuffer);
    	u.password[readBytes-1] = '\0';
    	int status=checkcredentials();
    	if(status)
    	{
    	bzero(writeBuffer, sizeof(writeBuffer));
    	strcpy(writeBuffer,"lOGIN SUCCESSFUL \n");
    	write(desc, writeBuffer, strlen(writeBuffer));
    	
    	}
    	else
    	{
    		bzero(writeBuffer, sizeof(writeBuffer));
    		strcpy(writeBuffer,"lOGIN FAILED \n");
    		write(desc, writeBuffer, strlen(writeBuffer));
    	}
    	return status;
    	
}

int checkcredentials()
{
	
	if(u.usertype=='F')
	{
		if(checkfaculty(u.loginid,u.password))
		{
			
			return 1;
		}
		else
		{
		perror("wrong credentials\n");
		return 0;
		}
	}
	else if(u.usertype=='A')
	{
		
		if(strcmp(u.loginid,a.loginid)==0 && strcmp(u.password,a.password)==0)
		{
			
			return 1;
		}
		else
		{
			perror("wrong credentials\n");
			return 0;
		}
		
	}
	else if(u.usertype=='S')
	{
		if(checkstudent(u.loginid,u.password))
		{
		
		return 1;
		}
		else
		{
		return 0;
		}
	}
	else
	{
		perror("incorrect user type\n");
		return 0;
	}
	return 1;
}

int checkfaculty(char* loginid,char* password)
{
 	
	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDONLY);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	struct flock readl;
	readl.l_type=F_RDLCK;
        readl.l_whence=SEEK_SET;
	readl.l_start=0;
	readl.l_len=0;
	int status=fcntl(facfd,F_SETLKW,&readl);
	if(status==-1)
	{
		perror("error in fcntl\n");
	}
	ssize_t bytesRead;
    // Read and process records in a loop
   	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
        // Process the record
       
         if(strcmp(faculty_detail.loginid,loginid)==0 && strcmp(faculty_detail.password,password)==0)
		  {
			 return 1;
		  }
   	 }
   	
	readl.l_type=F_UNLCK;
	fcntl(facfd, F_SETLKW, &readl);
    // Check for read errors
    	if (bytesRead == -1) {
		perror("Error reading file");
		close(facfd);
		return 0;
    	}

    	// Close the file
    	close(facfd);

    return 0;

   		
		
}

int checkstudent(char* loginid,char* password)
{
 	
	struct student stud_detail;
	int sfd=open("student.txt", O_RDONLY);
	if(sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	struct flock readl;
	readl.l_type=F_RDLCK;
        readl.l_whence=SEEK_SET;
	readl.l_start=0;
	readl.l_len=0;
	int status=fcntl(sfd,F_SETLKW,&readl);
	if(status==-1)
	{
		perror("error in fcntl\n");
	}
	ssize_t bytesRead;
	// Read and process records in a loop
   	 while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
        // Process the record
       
         if(strcmp(stud_detail.loginid,loginid)==0 && strcmp(stud_detail.password,password)==0)
		  {
			 return 1;
		  }
   	 }
   	
	readl.l_type=F_UNLCK;
	fcntl(sfd, F_SETLKW, &readl);
    // Check for read errors
    	if (bytesRead == -1) {
		perror("Error reading file");
		close(sfd);
		return 0;
    	}

    	// Close the file
    	close(sfd);

    return 0;
}


