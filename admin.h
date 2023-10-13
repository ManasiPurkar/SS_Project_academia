#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include "common.h"


int addfaculty();
int viewFaculty();
int modifyFaculty();
int addStudent();
int viewStudent();
int modifyStudent();
//for checking
/*int main()
{
	int status=addStudent();
	if(status)
	{
		printf("successfully added faculty\n");
	}
	else
	{
		printf("error increating faculty\n");
	}
	return 0;
}*/

int addfaculty(int desc)
{
	/*ssize_t readb;
	char readbuff[1000];*/
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Add Faculty\n");
	struct faculty lastf,newf;
	
	
	int facfd=open("faculty.txt", O_RDWR | O_CREAT | O_APPEND, 0766);
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
		ssize_t readb=read(facfd,&lastf,sizeof(struct faculty));
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
	strcat(writeBuffer,"Enter name: \n");
	/*ssize_t bytesRead = read(STDIN_FILENO, newf.name, sizeof(newf.name));
	if (bytesRead <= 1){
        perror("Error in storing name\n"); //equal to 1 for handling empty name
        return 0;
    	}*/
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing data to client!");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(newf.name,readBuffer);
    	newf.name[readBytes-1] = '\0';
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter email: \n");
	/*bytesRead = read(STDIN_FILENO, newf.email, sizeof(newf.email));
	if (bytesRead <= 1){
        perror("Error in storing email\n");
        return 0;
    	} */
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(newf.email,readBuffer);
    	newf.email[readBytes-1] = '\0';
    	//char mob[11];
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter mobile number: \n");
	/*bytesRead = read(STDIN_FILENO, mob, sizeof(mob));
    	if (bytesRead <= 10){
        perror("Error in storing mobile no.\n");
        return 0;
    	}*/
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
       
    	for(int i=0;i<10;i++)
    	{
    	newf.mobno[i]=readBuffer[i]- '0';
    	}
    	bzero(readBuffer, sizeof(readBuffer));
    	newf.no_of_offered_c=0;
	for(int i=0;i<20;i++)
	{
	newf.offered_courses[i]=0;
	}
	
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","id=", newf.id);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","login id=", newf.loginid);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","password=", newf.password);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","name=", newf.name);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","email=", newf.email);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
    	strcpy(writeBuffer,"\nmobile=");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	bzero(writeBuffer, sizeof(writeBuffer));
    	
    	 for (int i = 0; i < sizeof(newf.mobno) / sizeof(newf.mobno[0]); i++) {
        // Format each integer into a string and append it to the writeBuffer
        char temp[8];  // Assumes a maximum of 8 digits for each integer
        sprintf(temp, "%d", newf.mobno[i]);
        strcat(writeBuffer, temp);
	}
	strcat(writeBuffer,"\n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	bzero(writeBuffer, sizeof(writeBuffer));
	
	ssize_t bytesWrite=write(facfd,&newf,sizeof(struct faculty));
	if (bytesWrite > 0){
         bzero(readBuffer, sizeof(readBuffer));
    	 strcpy(writeBuffer,"Successfully Added\n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	close(facfd);
        return 1;
    	}
    	
    	close(facfd);
	return 0;
}

int modifyFaculty(int desc)
{
	ssize_t bytesRead;
	char reqloginid[11];
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Modify Faculty\n");
   	strcat(writeBuffer,"Enter login id of faculty you want to modify: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(reqloginid,readBuffer);
    	reqloginid[readBytes-1]='\0';
    	bzero(readBuffer, sizeof(readBuffer));
	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDWR);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	
    	// Read and process records in a loop
   	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,reqloginid)==0)
		  {
		  	int offset=lseek(facfd,-sizeof(struct faculty),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock writel;
			 writel.l_type=F_WRLCK;
			 writel.l_whence=SEEK_CUR;
		  	 writel.l_start=0;
		  	 writel.l_len=sizeof(struct faculty);
		 	 int status=fcntl(facfd,F_SETLKW,&writel);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
			}
			 /*printf("Enter name: \n");
			ssize_t bytesRead = read(STDIN_FILENO, faculty_detail.name, sizeof(faculty_detail.name));
			if (bytesRead <= 1){
      			  perror("Error in storing name\n"); //equal to 1 for handling empty name
       			  return 0;
    			}
    			faculty_detail.name[bytesRead-1] = '\0';
    			fflush(stdin);
    			printf("Enter email: \n");
			bytesRead = read(STDIN_FILENO, faculty_detail.email, sizeof(faculty_detail.email));
			if (bytesRead <= 1){
       				perror("Error in storing email\n");
        			return 0;
    			} 
    			faculty_detail.email[bytesRead-1] = '\0';
    			char mob[11];
    			printf("Enter mobile number: \n");
			bytesRead = read(STDIN_FILENO, mob, sizeof(mob));
    			if (bytesRead <= 10){
      			  perror("Error in storing mobile no.\n");
      			  return 0;
    			}
    			for(int i=0;i<10;i++)
    			{
    				faculty_detail.mobno[i]=mob[i]- '0';
    			}*/
    			
    			strcpy(writeBuffer,"Enter name: \n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
       		       if (writeBytes == -1)
       			 {
                		perror("Error while writing data to client!");
                		return 0;
       			 }
       			 bzero(writeBuffer, sizeof(writeBuffer));
       			 readBytes = read(desc, readBuffer, sizeof(readBuffer));
      			  if (readBytes == -1)
       			  {
         		       perror("Error while reading");
           		       return 0;
       			 }
      			  strcpy(faculty_detail.name,readBuffer);
    			faculty_detail.name[readBytes-1] = '\0';
    			bzero(readBuffer, sizeof(readBuffer));
    			strcpy(writeBuffer,"Enter email: \n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
       		       if (writeBytes == -1)
       			 {
                		perror("Error while writing data to client!");
                		return 0;
       			 }
       			 bzero(writeBuffer, sizeof(writeBuffer));
       			 readBytes = read(desc, readBuffer, sizeof(readBuffer));
      			  if (readBytes == -1)
       			  {
         		       perror("Error while reading");
           		       return 0;
       			 }
      			 strcpy(faculty_detail.email,readBuffer);
    			faculty_detail.email[readBytes-1] = '\0';
    			bzero(readBuffer, sizeof(readBuffer));
    			strcpy(writeBuffer,"Enter mobile number: \n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
       			 if (writeBytes == -1)
        		{
             		   perror("Error while writing");
              		  return 0;
        		}
       			 bzero(writeBuffer, sizeof(writeBuffer));
       			 readBytes = read(desc, readBuffer, sizeof(readBuffer));
       			 if (readBytes == -1)
        		 {
               		  perror("Error while reading");
              		  return 0;
      			  }   
    			for(int i=0;i<10;i++)
    			{
    			faculty_detail.mobno[i]=readBuffer[i]- '0';
    			}
    			bzero(readBuffer, sizeof(readBuffer));
    			
    			
    			
    			ssize_t bytesWrite=write(facfd,&faculty_detail,sizeof(struct faculty));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(facfd, F_SETLKW, &writel);
			 close(facfd);
			 bzero(readBuffer, sizeof(readBuffer));
    			 strcpy(writeBuffer,"Successfully Updated\n");
			 writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			 return 1;
		  }
   	 }
   	// readl.l_type=F_UNLCK;
	//fcntl(facfd, F_SETLKW, &readl);
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
int viewFaculty(int desc)
{
	ssize_t bytesRead;
	char reqloginid[11];
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to View Faculty\n");
   	strcat(writeBuffer,"Enter login id of faculty you want to view: \n");
   	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(reqloginid,readBuffer);
    	reqloginid[readBytes-1]='\0';
	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDONLY);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	
    	// Read and process records in a loop
   	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,reqloginid)==0)
		  {
			int offset=lseek(facfd,-sizeof(struct faculty),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock readl;
			 readl.l_type=F_RDLCK;
			 readl.l_whence=SEEK_CUR;
		  	 readl.l_start=0;
		  	 readl.l_len=sizeof(struct faculty);
		 	 int status=fcntl(facfd,F_SETLKW,&readl);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
				return 0;
			}
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%d","id=", faculty_detail.id);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","login id=", faculty_detail.loginid);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","name=", faculty_detail.name);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","email=", faculty_detail.email);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"\nmobile=");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    			bzero(writeBuffer, sizeof(writeBuffer));	
    			for (int i = 0; i <10; i++) {
       			 // Format each integer into a string and append it to the writeBuffer
       			 char temp[8];  // Assumes a maximum of 8 digits for each integer
       			 sprintf(temp, "%d", faculty_detail.mobno[i]);
       			 strcat(writeBuffer, temp);
			}
			strcat(writeBuffer,"\n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%d","No of offered courses=", faculty_detail.no_of_offered_c);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s","offered courses=");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));	
    			for (int i = 0; i <10; i++) {
       			 // Format each integer into a string and append it to the writeBuffer
       			 char temp[8];  // Assumes a maximum of 8 digits for each integer
       			 sprintf(temp, "%d", faculty_detail.offered_courses[i]);
       			 strcat(writeBuffer, temp);
       			 if (i <9) {
            			strcat(writeBuffer, " ");
       			 }
			}
			strcat(writeBuffer,"\n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			
			 
			  readl.l_type=F_UNLCK;
			 fcntl(facfd, F_SETLKW, &readl);
			 close(facfd);
			 return 1;
		  }
   	 }
   
    	// Check for read errors
    	if (bytesRead == -1) {
		perror("Error in reading file");
		close(facfd);
		return 0;
    	}

    	// Close the file
    	close(facfd);
    	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Error in View Faculty\n");
   	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	return 0;  
}

int addStudent(int desc)
{
	ssize_t readb;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Add Student\n");
	struct student lasts,news;
	
	
	int sfd=open("student.txt", O_RDWR | O_CREAT | O_APPEND, 0766);
	if(sfd==-1)
	{
		perror("error in opening student data\n");
		return 0;
	}
	if(lseek(sfd,0,SEEK_END)==0)
	{
	news.id=1;
	}
	else if(lseek(sfd,0,SEEK_END)==-1)
	{
		perror("error in lseek operation\n");
		return 0;
	}
	else
	{
		int offset=lseek(sfd,-sizeof(struct student),SEEK_END);
		if(offset==-1)
		{
			perror("error in reaching last record\n");
			return 0;
		}
		readb=read(sfd,&lasts,sizeof(struct student));
		if(readb==-1)
		{
			perror("error in getting last record\n");
			return 0;
		}
		else
		{
			news.id=lasts.id+1;
		}
	}
	//printf("%d",newf.id);
	char nid[10];
	sprintf(nid,"%d",news.id);
	/*printf("Enter course: \n");
	ssize_t bytesRead = read(STDIN_FILENO, news.course, sizeof(news.course));
	if (bytesRead <= 1){
        perror("Error in storing course name\n"); //equal to 1 for handling empty name
        return 0;
    	}*/
    	strcat(writeBuffer,"Enter course: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing data to client!");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(news.course,readBuffer);
	news.course[readBytes-1] = '\0';
	int totalLength = strlen(news.course) + strlen(nid) + 2; // +2 for the underscore and null terminator

   	 // Create a result array with enough space
    	char result[totalLength];

    	// Copy the first string into the result
    	strcpy(result, news.course);

    	// Concatenate an underscore
    	strcat(result, "_");

    	// Concatenate the second string
    	strcat(result, nid);
    	strcpy(news.loginid,result);
	
	char* password=generateRandomPassword();
	strcpy(news.password,password);
	free(password);
	/*printf("Enter name: \n");
	bytesRead = read(STDIN_FILENO, news.name, sizeof(news.name));
	if (bytesRead <= 1){
        perror("Error in storing name\n"); //equal to 1 for handling empty name
        return 0;
    	}
    	news.name[bytesRead-1] = '\0';
    	printf("Enter email: \n");
	bytesRead = read(STDIN_FILENO, news.email, sizeof(news.email));
	if (bytesRead <= 1){
        perror("Error in storing email\n");
        return 0;
    	} 
    	news.email[bytesRead-1] = '\0';
    	printf("Enter home state: \n");
	bytesRead = read(STDIN_FILENO, news.homestate, sizeof(news.homestate));
	if (bytesRead <= 1){
        perror("Error in storing home state\n");
        return 0;
    	} 
    	news.homestate[bytesRead-1] = '\0';
    	char mob[11];
    	printf("Enter mobile number: \n");
	bytesRead = read(STDIN_FILENO, mob, sizeof(mob));
    	if (bytesRead <= 10){
        perror("Error in storing mobile no.\n");
         return 0;
    	}
    	for(int i=0;i<10;i++)
    	{
    	news.mobno[i]=mob[i]- '0';
    	}
    	printf("Enter Age: \n");
    	char age[10];
   	 if (read(STDIN_FILENO, age, sizeof(age)) == -1) {
       		 perror("Error in reading age");
        	return 0;
    	}
	news.age = atoi(age);
	*/
	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter name: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(news.name,readBuffer);
    	news.name[readBytes-1] = '\0';
	
	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter email: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(news.email,readBuffer);
    	news.email[readBytes-1] = '\0';
    	
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter home state: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(news.homestate,readBuffer);
    	news.homestate[readBytes-1] = '\0';
    	//char mob[11];
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter mobile number: \n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
       
    	for(int i=0;i<10;i++)
    	{
    	news.mobno[i]=readBuffer[i]- '0';
    	}
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Enter age: \n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
	news.age=atoi(readBuffer);
	for(int i=0;i<10;i++)
	{
	news.enrolled_courses[i]=0;
	}
	
	/*printf("id=%d\n",news.id);
	printf("loginid=%s\n",news.loginid);
	printf("password=%s\n",news.password);
	printf("course=%s \n",news.course);
	printf("name=%s \n",news.name);
    	printf("email=%s \n",news.email);
    	printf("age=%d\n",news.age);
    	printf("home state=%s \n",news.homestate);
    	printf("mobile=");
    	for(int i=0;i<10;i++)
    	{
    	printf("%d",news.mobno[i]);
    	}
	printf("\n");*/
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","id=", news.id);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","login id=", news.loginid);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","password=", news.password);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","name=", news.name);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","email=", news.email);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","home state=", news.homestate);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","age=", news.age);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
    	strcpy(writeBuffer,"\nmobile=");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	bzero(writeBuffer, sizeof(writeBuffer));
    	
    	 for (int i = 0; i < 10; i++) {
        // Format each integer into a string and append it to the writeBuffer
        char temp[8];  // Assumes a maximum of 8 digits for each integer
        sprintf(temp, "%d", news.mobno[i]);
        strcat(writeBuffer, temp);
	}
	strcat(writeBuffer,"\n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	bzero(writeBuffer, sizeof(writeBuffer));
	
	
	ssize_t bytesWrite=write(sfd,&news,sizeof(struct student));
	if (bytesWrite > 0){
         bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Successfully Added\n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        close(sfd);
        return 1;
    	}
    	
    	close(sfd);
	return 0;
}

int viewStudent(int desc)
{
	/*ssize_t bytesRead;
	char reqloginid[11];
	printf("Enter login id of student you want to view: \n");
	bytesRead = read(STDIN_FILENO, reqloginid, sizeof(reqloginid));
	// Check if reading was successful 
    	if (bytesRead <= 1) {
        perror("Error in storing login id\n");
        return 0;
    	}
    	reqloginid[bytesRead-1]='\0';*/
    	ssize_t bytesRead;
	char reqloginid[11];
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to View Student\n");
   	strcat(writeBuffer,"Enter login id of student you want to view: \n");
   	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(reqloginid,readBuffer);
    	reqloginid[readBytes-1]='\0';
	struct student stud_detail;
	int sfd=open("student.txt", O_RDONLY);
	if(sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	// Read and process records in a loop
   	 while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(stud_detail.loginid,reqloginid)==0)
		  {
			int offset=lseek(sfd,-sizeof(struct student),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock readl;
			 readl.l_type=F_RDLCK;
			 readl.l_whence=SEEK_CUR;
		  	 readl.l_start=0;
		  	 readl.l_len=sizeof(struct student);
		 	 int status=fcntl(sfd,F_SETLKW,&readl);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
			}
			/* printf("id= %d\n",stud_detail.id);
			 printf("course= %s\n",stud_detail.course);
			 printf("loginid= %s\n",stud_detail.loginid);
			 printf("name= %s\n",stud_detail.name);
			 printf("email id= %s\n",stud_detail.email);
			 printf("age= %d\n",stud_detail.age);
			 printf("home state= %s\n",stud_detail.homestate);
			 printf("mobile=");
			 for(int i=0;i<10;i++)
			 {
			    	printf("%d",stud_detail.mobno[i]);
			 }
			 printf("\n");
			 printf("Enrolled courses=");
			 for(int i=0;i<10;i++)
			 {
			    	printf("%d",stud_detail.enrolled_courses[i]);
			 }*/
			 
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%d","id=", stud_detail.id);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","course=", stud_detail.course);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","login id=", stud_detail.loginid);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","name=", stud_detail.name);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","email=", stud_detail.email);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"\nmobile=");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    			bzero(writeBuffer, sizeof(writeBuffer));	
    			for (int i = 0; i <10; i++) {
       			 // Format each integer into a string and append it to the writeBuffer
       			 char temp[8];  // Assumes a maximum of 8 digits for each integer
       			 sprintf(temp, "%d", stud_detail.mobno[i]);
       			 strcat(writeBuffer, temp);
			}
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s","enrolled courses=");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));	
    			for (int i = 0; i <10; i++) {
       			 // Format each integer into a string and append it to the writeBuffer
       			 char temp[8];  // Assumes a maximum of 8 digits for each integer
       			 sprintf(temp, "%d", stud_detail.enrolled_courses[i]);
       			 strcat(writeBuffer, temp);
       			 if (i <9) {
            			strcat(writeBuffer, " ");
       			 }
			}
			strcat(writeBuffer,"\n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
				 
			 
			 readl.l_type=F_UNLCK;
			 fcntl(sfd, F_SETLKW, &readl);
			 close(sfd);
			 return 1;
		  }
   	 }
   	
    	// Check for read errors
    	if (bytesRead == -1) {
		perror("Error in reading file");
		close(sfd);
		return 0;
    	}

    	// Close the file
    	bzero(readBuffer, sizeof(readBuffer));
    	strcpy(writeBuffer,"Failed to view student\n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
    	close(sfd);
    	return 0;  
}
int modifyStudent(int desc)
{
	/*ssize_t bytesRead;
	char reqloginid[11];
	printf("Enter login id of student you want to modify: \n");
	bytesRead = read(STDIN_FILENO, reqloginid, sizeof(reqloginid));
	// Check if reading was successful 
    	if (bytesRead <= 1) {
        perror("Error in storing login id\n");
        return 0;
    	}
    	reqloginid[bytesRead-1]='\0';*/
    	ssize_t bytesRead;
	char reqloginid[11];
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Modify Student\n");
   	strcat(writeBuffer,"Enter login id of student you want to modify: \n");
    	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading");
                return 0;
        }
        strcpy(reqloginid,readBuffer);
    	reqloginid[readBytes-1]='\0';
    	bzero(readBuffer, sizeof(readBuffer));
	struct student stud_detail;
	int sfd=open("student.txt", O_RDWR);
	if(sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	// Read and process records in a loop
   	 while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(stud_detail.loginid,reqloginid)==0)
		  {
		  	int offset=lseek(sfd,-sizeof(struct student),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock writel;
			 writel.l_type=F_WRLCK;
			 writel.l_whence=SEEK_CUR;
		  	 writel.l_start=0;
		  	 writel.l_len=sizeof(struct student);
		 	 int status=fcntl(sfd,F_SETLKW,&writel);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
			}
			/*printf("Enter course: \n");
			 bytesRead = read(STDIN_FILENO, stud_detail.course, sizeof(stud_detail.course));
			if (bytesRead <= 1){
      			  perror("Error in storing name\n"); //equal to 1 for handling empty name
       			  return 0;
    			}
    			stud_detail.course[bytesRead-1] = '\0';*/
    			
    			strcat(writeBuffer,"Enter course: \n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
      			 if (writeBytes == -1)
       			 {
           		     perror("Error while writing data to client!");
             		     return 0;
      			  }
       			 bzero(writeBuffer, sizeof(writeBuffer));
      		         readBytes = read(desc, readBuffer, sizeof(readBuffer));
     		          if (readBytes == -1)
     		         {
         		       perror("Error while reading");
            		       return 0;
      			  }
      		         strcpy(stud_detail.course,readBuffer);
			stud_detail.course[readBytes-1] = '\0';
    			bzero(readBuffer, sizeof(readBuffer));
    				
    			char nid[10];
			sprintf(nid,"%d",stud_detail.id);
    			int totalLength = strlen(stud_detail.course) + strlen(nid) + 2; // +2 for the underscore and null terminator

   			 // Create a result array with enough space
    			char result[totalLength];

    			// Copy the first string into the result
    			strcpy(result, stud_detail.course);

    			// Concatenate an underscore
    			strcat(result, "_");

    			// Concatenate the second string
    			strcat(result, nid);
    			strcpy(stud_detail.loginid,result);
	
			/*printf("Enter name: \n");
			bytesRead = read(STDIN_FILENO, stud_detail.name, sizeof(stud_detail.name));
			if (bytesRead <= 1){
      			  perror("Error in storing name\n"); //equal to 1 for handling empty name
       			  return 0;
    			}
    			stud_detail.name[bytesRead-1] = '\0';
    			fflush(stdin);*/
    			strcat(writeBuffer,"Enter name: \n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
      			 if (writeBytes == -1)
       			 {
           		     perror("Error while writing data to client!");
             		     return 0;
      			  }
       			 bzero(writeBuffer, sizeof(writeBuffer));
      		         readBytes = read(desc, readBuffer, sizeof(readBuffer));
     		          if (readBytes == -1)
     		         {
         		       perror("Error while reading");
            		       return 0;
      			  }
      		        strcpy(stud_detail.name,readBuffer);
			stud_detail.name[readBytes-1] = '\0';
    			bzero(readBuffer, sizeof(readBuffer));
    			
    			/*printf("Enter email: \n");
			bytesRead = read(STDIN_FILENO, stud_detail.email, sizeof(stud_detail.email));
			if (bytesRead <= 1){
       				perror("Error in storing email\n");
        			return 0;
    			} 
    			stud_detail.email[bytesRead-1] = '\0';*/
    			strcat(writeBuffer,"Enter email: \n");
    			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
      			 if (writeBytes == -1)
       			 {
           		     perror("Error while writing data to client!");
             		     return 0;
      			  }
       			 bzero(writeBuffer, sizeof(writeBuffer));
      		         readBytes = read(desc, readBuffer, sizeof(readBuffer));
     		          if (readBytes == -1)
     		         {
         		       perror("Error while reading");
            		       return 0;
      			  }
      		         strcpy(stud_detail.email,readBuffer);
			stud_detail.email[readBytes-1] = '\0';
    			bzero(readBuffer, sizeof(readBuffer));
    			
    			/*char mob[11];
    			printf("Enter mobile number: \n");
			bytesRead = read(STDIN_FILENO, mob, sizeof(mob));
    			if (bytesRead <= 10){
      			  perror("Error in storing mobile no.\n");
      			  return 0;
    			}
    			for(int i=0;i<10;i++)
    			{
    				stud_detail.mobno[i]=mob[i]- '0';
    			}*/
    			bzero(readBuffer, sizeof(readBuffer));
    			strcpy(writeBuffer,"Enter mobile number: \n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
       		        if (writeBytes == -1)
       			 {
            		    perror("Error while writing");
             		    return 0;
      			  }
       			 bzero(writeBuffer, sizeof(writeBuffer));
       			 readBytes = read(desc, readBuffer, sizeof(readBuffer));
       			 if (readBytes == -1)
       			  {
             		   perror("Error while reading");
              		  return 0;
        		 }
    			 for(int i=0;i<10;i++)
    			{
    			 stud_detail.mobno[i]=readBuffer[i]- '0';
    			}
    			
    			
    			ssize_t bytesWrite=write(sfd,&stud_detail,sizeof(struct student));
			 writel.l_type=F_UNLCK;
			 fcntl(sfd, F_SETLKW, &writel);
			 close(sfd);
			 if (bytesWrite > 0){
       			  bzero(readBuffer, sizeof(readBuffer));
    			  strcpy(writeBuffer,"Successfully modified student\n");
			  writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
      			  return 1;
    			}
    			else 
    			{
    			 perror("error in writing struct\n");
			 return 0;
			 }
		  }
   	 }
   	 if (bytesRead == -1) {
		perror("Error reading file");
		close(sfd);
		return 0;
    	}

    	// Close the file
    	close(sfd);
    	return 0;  	 
}
