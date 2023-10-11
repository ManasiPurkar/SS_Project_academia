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
//for checking
/*int main()
{
	
	int status=login();
	if(status)
	{
	printf("successfully logged in\n");
	}
	else
	{
	printf("error in log in\n");
	}
	logout();
	return 0;
}*/

int login()
{
	
	printf("Welcome to login window...\n");
	printf("Available users are 1.Admin(A)  2.Faculty(F)   3.Student(S)\n");
  	printf("Enter user type char: \n");
	ssize_t bytesRead = read(STDIN_FILENO, &u.usertype, sizeof(u.usertype));
	if (bytesRead <= 0) {
        perror("Error in storing usertype\n");
        return 0;
    	}
	if (u.usertype != 'A' && u.usertype != 'F' && u.usertype != 'S') {
        printf("Invalid user type. Please enter A, F, or S.\n");
        return 0;
   	}
   	int c;

    	// Flush the input buffer by reading and discarding characters until a newline or EOF is encountered
	while ((c = getchar()) != '\n' && c != EOF) {
		// Discard the character
	    }
    	printf("Enter Login id: \n");
    	bytesRead = read(STDIN_FILENO, u.loginid, sizeof(u.loginid));
	// Check if reading was successful 
    	if (bytesRead <= 1) {
        perror("Error in storing login id\n");
        return 0;
    	}
    	u.loginid[bytesRead-1] = '\0';
    	printf("Enter password: \n");
    	bytesRead = read(STDIN_FILENO, u.password, sizeof(u.password));
	// Check if reading was successful 
    	if (bytesRead <= 1) {
        perror("Error in storing password\n");
        return 0;
    	}
    	u.password[bytesRead-1] = '\0';
    	int status=checkcredentials();
    	return status;
    	
}

int checkcredentials()
{
	
	if(u.usertype=='F')
	{
		if(checkfaculty(u.loginid,u.password))
		{
			u.authenticated=1;
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
			u.authenticated=1;
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
		u.authenticated=1;
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
	/*
    	char *token;

    	// Tokenize the string
    	token = strtok(loginid, "_");

   	 // Check if there are enough tokens
    	if (token != NULL) {
		// Skip the first token
		token = strtok(NULL, " ");
		// Check if there is a second token
		if (token != NULL) {
		    
   		 char *endptr; // Used to check for conversion errors
    		 long ID = strtol(token, &endptr, 10);
    
   		 if (*endptr != '\0' && *endptr != '\n') {
      		  printf("Conversion failed. Invalid characters found: %s\n", endptr);
  		  return false;
  		  }
    		  off_t recordoff=(off_t)(sizeof(struct faculty) * (ID -1));
		  struct flock readl;
		  readl.l_type=F_RDLCK;
		  readl.l_whence=SEEK_SET;
		  readl.l_start=recordoff;
		  readl.l_len=sizeof(struct faculty);
		  int status=fcntl(facfd,F_SETLKW,&readl);
		  ssize_t readb=read(fd, &faculty_detail, sizeof(faculty));
		  if(readb==-1)
		  {
		  	perror("error in reading record\n");
		  	return false;
		  }
		  if(faculty_detail.loginid==loginid && faculty_detail.password==password)
		  {
			 return true;
		  }
		  readl.l_type=F_UNLCK;
		  fcntl(facfd, F_SETLKW, &readl);
		  close(facfd);
		    
		} else {
		    printf("There is no second token.\n");
		}
    	} 
    	else {
        	printf("There are no tokens in the input string.\n");
    	}
	*/
		
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


void logout()
{
	if(u.authenticated==1)
	{
		u.authenticated=0;
		printf("Logged out successfully\n");
	}
	else
	{
		printf("user not logged in\n");
	}	
}
