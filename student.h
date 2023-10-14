/*Name- Manasi Purkar
Roll- MT2023158
File- Student fun
*/
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>


int view_courses(int desc);
int enroll_course(int desc);
int unenroll_course(int desc);
int unenroll(int stdid,int cid);
int view_enrolled_courses(int desc);
int change_password_stud(int desc);

int view_courses(int desc)
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
	struct course course_detail;
	int cfd=open("course.txt", O_RDONLY);
	if(cfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	struct flock readl;
	readl.l_type=F_RDLCK;
        readl.l_whence=SEEK_SET;
	readl.l_start=0;
	readl.l_len=0;
	int status=fcntl(cfd,F_SETLKW,&readl);
	if(status==-1)
	{
		perror("error in fcntl\n");
		return 0;
	}
	ssize_t bytesRead;
	 while ((bytesRead = read(cfd, &course_detail, sizeof(struct course))) > 0)
   	  {
        // Print the record
       		
       		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%d","id=", course_detail.id);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%s","name=", course_detail.name);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%s","dept=", course_detail.dept);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%d","credits=", course_detail.credits);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%s","offered by=", course_detail.offered_by);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%d","Total Seats=", course_detail.total_seats);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		sprintf(writeBuffer, "\n%s%d","Enrolled Seats=", course_detail.enrolled_seats);
		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		bzero(writeBuffer, sizeof(writeBuffer));
		
		strcat(writeBuffer,"\n");
    		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
       		bzero(writeBuffer, sizeof(writeBuffer));  		
   	 }
   	 readl.l_type=F_UNLCK;
	fcntl(cfd, F_SETLKW, &readl);
    // Check for read errors
    	if (bytesRead == -1) {
		perror("Error reading file");
		close(cfd);
		return 0;
    	}

    	// Close the file
    	close(cfd);

    return 1;
}

int enroll_course(int desc)
{	
	
	int cid;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Enroll course\n");
   	strcat(writeBuffer,"Enter course id which you want to enroll= \n");
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
        cid = atoi(readBuffer);
        bzero(readBuffer, sizeof(readBuffer));
	
	ssize_t bytesRead;
	//printf("reqc=%d\n",cid);
	struct course course_detail;
	struct student stud_detail;
	int cfd=open("course.txt", O_RDWR);
	int sfd=open("student.txt", O_RDWR);
	if(cfd==-1 || sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	int studfound=0;
	int stdid;
	while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(stud_detail.loginid,u.loginid)==0)
		  {
		  	studfound=1;
		  	stdid=stud_detail.id;
		  	break;
		  }
   	 }
	int coursefound=0;
	 while ((bytesRead = read(cfd, &course_detail, sizeof(struct course))) > 0)
   	  {
       		 // Process the record
        	 if(course_detail.id==cid)
		  {
		  	coursefound=1;
		  	break;
		  }
   	 }
   	 if(coursefound==1)
   	 {
   	 	int offset=lseek(cfd,-sizeof(struct course),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock writel;
			 writel.l_type=F_WRLCK;
			 writel.l_whence=SEEK_CUR;
		  	 writel.l_start=0;
		  	 writel.l_len=sizeof(struct course);
		 	 int status=fcntl(cfd,F_SETLKW,&writel);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
			}
			
  			int empty=-1;
  			for(int i=0;i<200;i++)
  			{
  				if(course_detail.enrolled_stud[i]==stdid)
  				{
  					bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   					strcpy(writeBuffer,"Course already enrolled\n");
					writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
  					return 0;
  				}
  				else if(empty==-1 && course_detail.enrolled_stud[i]==0)
  				{
  					empty=i;
  				}
  			}
  			
			course_detail.enrolled_stud[empty]=stdid;
			course_detail.enrolled_seats+=1;
			ssize_t bytesWrite=write(cfd,&course_detail,sizeof(struct course));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(cfd, F_SETLKW, &writel);
			 close(cfd);
			 
   	 }
   	 else
   	 {
   	 	printf("course not found\n");
   	 	return 0;
   	 }
   	  if(studfound==1)
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
			
  			for(int i=0;i<10;i++)
  			{
			if(stud_detail.enrolled_courses[i]==0)
			{
				stud_detail.enrolled_courses[i]=cid;
				break;
			}
			}
			ssize_t bytesWrite=write(sfd,&stud_detail,sizeof(struct student));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(sfd, F_SETLKW, &writel);
			 close(sfd);
			 return 1;
		}
   	 return 0;	
}

int unenroll_course(int desc)
{
	
	int cid;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Enroll course\n");
   	strcat(writeBuffer,"Enter course id which you want to enroll= \n");
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
        cid = atoi(readBuffer);
        bzero(readBuffer, sizeof(readBuffer));
	int stdid;
	char delimiter[] = "_";
	char *token = strtok(u.loginid, delimiter); //to get student id from its login id
    	while (token != NULL) {
      	// Move to the next token
      	token = strtok(NULL, delimiter);
       	if (token != NULL) {
         	stdid=atoi(token);
        	}
  	 }
	int status=unenroll(stdid,cid);
	if(status)
	{
	return 1;
	}
	return 0;
}
int unenroll(int stdid,int cid)
{
	
	//printf("reqc=%d\n",cid);
	printf("hello");
	ssize_t bytesRead;
	struct course course_detail;
	struct student stud_detail;
	int cfd=open("course.txt", O_RDWR);
	int sfd=open("student.txt", O_RDWR);
	if(cfd==-1 || sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	int studfound=0;

	while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	 if(stud_detail.id==stdid)
		  {
		  	studfound=1;
		  	break;
		  }
   	 }
   	 int coursefound=0;
	 while ((bytesRead = read(cfd, &course_detail, sizeof(struct course))) > 0)
   	  {
       		 // Process the record
        	 if(course_detail.id==cid)
		  {
		  	coursefound=1;
		  	break;
		  }
   	 }
   	 if(coursefound==1)
   	 {
   	 	int offset=lseek(cfd,-sizeof(struct course),SEEK_CUR);
			if(offset==-1)
			{
				perror("error in reaching required record\n");
				return 0;
			}
			struct flock writel;
			 writel.l_type=F_WRLCK;
			 writel.l_whence=SEEK_CUR;
		  	 writel.l_start=0;
		  	 writel.l_len=sizeof(struct course);
		 	 int status=fcntl(cfd,F_SETLKW,&writel);
		 	 if(status==-1)
			{
				perror("error in fcntl\n");
			}
			for(int i=0;i<course_detail.total_seats;i++)
  			{
  				if(course_detail.enrolled_stud[i]==stdid)
  				{
  					course_detail.enrolled_stud[i]=0;
  					break;
  				}
  			}
  			
			course_detail.enrolled_seats-=1;
			ssize_t bytesWrite=write(cfd,&course_detail,sizeof(struct course));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(cfd, F_SETLKW, &writel);
			 close(cfd);
			 
   	 }
   	 else
   	 {
   	 	printf("course not found\n");
   	 	return 0;
   	 }
   	  if(studfound==1)
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
			for(int i=0;i<10;i++)
  			{
			if(stud_detail.enrolled_courses[i]==cid)
			{
				stud_detail.enrolled_courses[i]=0;
				break;
			}
			}
			ssize_t bytesWrite=write(sfd,&stud_detail,sizeof(struct student));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(sfd, F_SETLKW, &writel);
			 close(sfd);
			 printf("hello");			
			 return 1;
		}
		
	printf("byee");
	
   	 return 0;	
}
int view_enrolled_courses(int desc)
{
	ssize_t bytesRead;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
	struct student stud_detail;
	int sfd=open("student.txt", O_RDWR);
	if(sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	int studfound=0;

	while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	  if(strcmp(stud_detail.loginid,u.loginid)==0)
		  {
		  	studfound=1;
		  	break;
		  }
   	 }
   	if(studfound)
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
			
			sprintf(writeBuffer, "\n%s","Enrolled courses=");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			bzero(writeBuffer, sizeof(writeBuffer));	
    			for (int i = 0; i <10; i++) {
       			 // Format each integer into a string and append it to the writeBuffer
       			 char temp[8];  // Assumes a maximum of 8 digits for each integer
       			 sprintf(temp, "%d", stud_detail.enrolled_courses[i]);
       			 strcat(writeBuffer, temp);
       			 if (i < 9) {
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
   	return 0;
}

int change_password_stud(int desc)
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
	struct student stud_detail;
	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Change Password\n");
	int sfd=open("student.txt", O_RDWR);
	if(sfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	ssize_t bytesRead;
	 while ((bytesRead = read(sfd, &stud_detail, sizeof(struct student))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(stud_detail.loginid,u.loginid)==0)
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
			strcat(writeBuffer,"Enter new Password\n");
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
        		strcpy(stud_detail.password,readBuffer);
    			stud_detail.password[readBytes-1] = '\0';
    			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","Changed Password=", stud_detail.password);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	
    			bzero(readBuffer, sizeof(readBuffer));
    			ssize_t bytesWrite=write(sfd,&stud_detail,sizeof(struct student));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
    			 writel.l_type=F_UNLCK;
			 fcntl(sfd, F_SETLKW, &writel);
			 close(sfd);
			 bzero(readBuffer, sizeof(readBuffer));
    			 strcpy(writeBuffer,"\nSuccessfully Updated\n");
			 writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			 return 1;
		  }
   	 }
   	 return 0;
   }
