#include<stdio.h>
#include<fcntl.h>
#include <string.h>

#include "structures.h"

bool checkcredentials(char* loginid, char* password)
{
	if(loginid[0]=='F')
	{
		if(checkfaculty(loginid,password))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
	else if(loginid[0]=='A')
	{
		struct admin a;
		char* apassword=a.password;
		if(loginid==a.loginid && password==apassword)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		if(checkstudent(loginid,password))
		{
		return true;
		}
		else
		{
		return false;
		}
	}
}

bool checkfaculty(char* loginid,char* password)
{
 	
	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDONLY);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return false;
	}
	struct flock readl;
	readl.l_type=F_RDLCK;
        readl.l_whence=SEEK_SET;
	readl.l_start=0;
	readl.l_len=0;
	int status=fcntl(facfd,F_SETLKW,&readl);
	ssize_t bytesRead;
    // Read and process records in a loop
   	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
        // Process the record (in this example, we simply print it)
         if(faculty_detail.loginid==loginid && faculty_detail.password==password)
		  {
			 return true;
		  }
   	 }
	readl.l_type=F_UNLCK;
	fcntl(facfd, F_SETLKW, &readl);
    // Check for read errors
    	if (bytesRead == -1) {
		perror("Error reading file");
		close(facfd);
		return false;
    	}

    	// Close the file
    	close(facfd);

    return false;
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
 
