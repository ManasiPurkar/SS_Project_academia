#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>


#include "student.h"

int view_offering_courses(int desc); //show more data in this if possible
int add_new_course(int desc);
int remove_course(int desc);
int update_course(int desc);
int change_password(int desc);

int view_offering_courses(int desc)
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDONLY);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	ssize_t bytesRead;
	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,u.loginid)==0)
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
			}
			char readBuffer[1000],writeBuffer[1000];
   			ssize_t readBytes, writeBytes; 
   			bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   			sprintf(writeBuffer, "\n%s%d","No. of offered courses=", faculty_detail.no_of_offered_c);
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
   	 if (bytesRead == -1) {
		perror("Error in reading file");
		close(facfd);
		return 0;
    	}
	bzero(writeBuffer, sizeof(writeBuffer));
	strcpy(writeBuffer, "Error in showing offered courses");
	write(desc, writeBuffer, strlen(writeBuffer));
    	// Close the file
    	close(facfd);
    	return 0;  
}
int add_new_course(int desc)
{
	
	ssize_t readb;
	//char readbuff[1000];
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Add New Course\n");
	struct course lastc,newc;
	
	
	int cfd=open("course.txt", O_RDWR | O_CREAT | O_APPEND, 0766);
	if(cfd==-1)
	{
		perror("error in opening course data\n");
		return 0;
	}
	if(lseek(cfd,0,SEEK_END)==0)
	{
	newc.id=1;
	}
	else if(lseek(cfd,0,SEEK_END)==-1)
	{
		perror("error in lseek operation\n");
		return 0;
	}
	else
	{
		int offset=lseek(cfd,-sizeof(struct course),SEEK_END);
		if(offset==-1)
		{
			perror("error in reaching last record\n");
			return 0;
		}
		struct flock readl;
		readl.l_type=F_RDLCK;
		readl.l_whence=SEEK_CUR;
		readl.l_start=0;
		readl.l_len=sizeof(struct course);
		int status=fcntl(cfd,F_SETLKW,&readl);
		if(status==-1)
		{
			perror("error in fcntl\n");
		}
		readb=read(cfd,&lastc,sizeof(struct course));
		if(readb==-1)
		{
			perror("error in getting last record\n");
			return 0;
		}
		else
		{
			newc.id=lastc.id+1;
		}
		readl.l_type=F_UNLCK;
		fcntl(cfd, F_SETLKW, &readl);
	}
	
    	strcat(writeBuffer,"Enter course name: \n");
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
        strcpy(newc.name,readBuffer);
    	newc.name[readBytes-1] = '\0';
    	bzero(readBuffer, sizeof(readBuffer));
    
    	strcat(writeBuffer,"Enter department: \n");
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
        strcpy(newc.dept,readBuffer);
    	newc.dept[readBytes-1] = '\0';
    	bzero(readBuffer, sizeof(readBuffer));
    	
	strcat(writeBuffer,"Enter total seats: \n");
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
    	newc.total_seats = atoi(readBuffer);
	bzero(readBuffer, sizeof(readBuffer));
	
    	newc.enrolled_seats=0;
    	strcpy(newc.offered_by,u.loginid);
	for(int i=0;i<200;i++)
	{
	newc.enrolled_stud[i]=0;
	}
	
	strcat(writeBuffer,"Enter credits: \n");
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
    	newc.credits = atoi(readBuffer);
	bzero(readBuffer, sizeof(readBuffer));
	
	
    	
    	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","id=", newc.id);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","name=", newc.name);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","dept=", newc.dept);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","total seats=", newc.total_seats);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%d","credits=", newc.credits);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	sprintf(writeBuffer, "\n%s%s","offered by=", newc.offered_by);
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	bzero(writeBuffer, sizeof(writeBuffer));
	
	ssize_t bytesWrite=write(cfd,&newc,sizeof(struct course));
	if (bytesWrite <= 0){
        perror("Error in writing struct\n");
        return 0;
    	}
    	close(cfd);
    	
    	//make changes to faculty struct
    	struct faculty faculty_detail;
    	ssize_t bytesRead;
	int facfd=open("faculty.txt", O_RDWR);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,u.loginid)==0)
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
				return 0;
			}
			for(int i=0;i<10;i++)
			{if(faculty_detail.offered_courses[i]==0)
			{
				faculty_detail.offered_courses[i]=newc.id;
				break;
			}
			}
			faculty_detail.no_of_offered_c+=1;
			ssize_t bytesWrite=write(facfd,&faculty_detail,sizeof(struct faculty));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(facfd, F_SETLKW, &writel);
			 close(facfd);
			 bzero(writeBuffer, sizeof(writeBuffer));
			 strcat(writeBuffer,"Course added successfully \n");
    			 writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			 return 1;
		  }
   	 }
   	 if (bytesRead == -1) {
		perror("Error in reading file");
		close(facfd);
		return 0;
    	}

    	// Close the file
    	close(facfd);
    	  
			
	return 0;
}

int remove_course(int desc)
{
	int cid;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Remove course\n");
   	strcat(writeBuffer,"Enter course id which you want to remove= \n");
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
	int cfd=open("course.txt", O_RDONLY);
	if(cfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	
	
	int tempFd = open("temp.txt", O_WRONLY | O_CREAT, 0666);
   	 if (tempFd == -1) {
        perror("Error creating temporary file");
        close(cfd);
        return 0;
    	}

   	 
   	 int found = 0;
	
    // Read and process records in a loop
   	 while ((bytesRead = read(cfd, &course_detail, sizeof(struct course))) > 0)
   	  {
        // Process the record
        	 if(course_detail.id==cid)
		  {
			 found=1;
		  }
		  else
		  {
		  	 write(tempFd, &course_detail, sizeof(struct course));
		  }
   	 }
   	 close(cfd);
   	 close(tempFd);

   	 // Remove the original file
    	if (remove("course.txt") != 0) {
        perror("Error deleting original file");
        return 0;
    	}

   	 // Rename the temporary file to the original filename
    	if (rename("temp.txt", "course.txt") != 0) {
        perror("Error renaming temporary file");
        return 0;
    	}

    	if (found) {
    	
    	//make changes to faculty struct
    	struct faculty faculty_detail;
	int facfd=open("faculty.txt", O_RDWR);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,u.loginid)==0)
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
				return 0;
			}
			for(int i=0; i<10 ; i++)
			{
				if(faculty_detail.offered_courses[i]==cid)
				{
					faculty_detail.offered_courses[i]=0;
					faculty_detail.no_of_offered_c-=1;
					break;
				}
			}	
			
			ssize_t bytesWrite=write(facfd,&faculty_detail,sizeof(struct faculty));
			if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    			}
			 writel.l_type=F_UNLCK;
			 fcntl(facfd, F_SETLKW, &writel);
			 close(facfd);
			 bzero(writeBuffer, sizeof(writeBuffer));
			 strcat(writeBuffer,"Course removed successfully \n");
    			 writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			 return 1;
		  }
   	 }
    	 if (bytesRead == -1) {
		perror("Error in reading file");
		close(facfd);
		return 0;
    	}

    	// Close the file
    	close(facfd);
    	perror("course not removed from faculty data\n");
        return 0; 
        
   	} 
   	 return 0;
}
int update_course(int desc)
{
	
	int cid;
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Update course\n");
   	strcat(writeBuffer,"Enter course id which you want to update= \n");
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
	int cfd=open("course.txt", O_RDWR);
	if(cfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	int found = 0;
	
    // Read and process records in a loop
   	 while ((bytesRead = read(cfd, &course_detail, sizeof(struct course))) > 0)
   	  {
        // Process the record
        	 if(course_detail.id==cid)
		  {
			 found=1;
			 break;
		  }
		 
   	 }
	if(found)
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
		
    		strcat(writeBuffer,"Enter course name: \n");
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
        	strcpy(course_detail.name,readBuffer);
    		course_detail.name[readBytes-1] = '\0';
    		bzero(readBuffer, sizeof(readBuffer));
    		
    		strcpy(writeBuffer,"Enter deepartment: \n");
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
        	strcpy(course_detail.dept,readBuffer);
    		course_detail.dept[readBytes-1] = '\0';
    		bzero(readBuffer, sizeof(readBuffer));
    		
    		
    		strcpy(writeBuffer,"Enter credits: \n");
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
    		course_detail.credits= atoi(readBuffer);
    		bzero(readBuffer, sizeof(readBuffer));
    		
    		
    		int dec_seats;
    		
    		strcpy(writeBuffer,"Decrease total seats by: \n");
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
    		dec_seats= atoi(readBuffer);
    		bzero(readBuffer, sizeof(readBuffer));
    		
    		course_detail.total_seats-=dec_seats;
    		for(int i=course_detail.total_seats; i<course_detail.enrolled_seats; i++)
    		{
    			unenroll(course_detail.enrolled_stud[i],course_detail.id); //unenroll course  
    			course_detail.enrolled_stud[i]=0;
    		}
    		if(course_detail.total_seats<course_detail.enrolled_seats)
    		{
 			   	course_detail.enrolled_seats=course_detail.total_seats;	
    		}
    		int inc_seats;
    		
    		strcpy(writeBuffer,"Increase total seats by: \n");
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
    		inc_seats= atoi(readBuffer);
    		bzero(readBuffer, sizeof(readBuffer));
    		
    		course_detail.total_seats+=inc_seats;
    		ssize_t bytesWrite=write(cfd,&course_detail,sizeof(struct course));
		if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    		}
		writel.l_type=F_UNLCK;
		fcntl(cfd, F_SETLKW, &writel);
		close(cfd);
		bzero(writeBuffer, sizeof(writeBuffer));
		strcat(writeBuffer,"Course updated successfully \n");
    		writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
		return 1;
	}
	else
	{
	 printf("course not found\n");
	 return 0;
	}
	return 0;	
}

int change_password(int desc)
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
	struct faculty faculty_detail;
	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
   	strcpy(writeBuffer,"Welcome to Change Password\n");
	int facfd=open("faculty.txt", O_RDWR);
	if(facfd==-1)
	{
		perror("error in opening\n");
		return 0;
	}
	ssize_t bytesRead;
	 while ((bytesRead = read(facfd, &faculty_detail, sizeof(struct faculty))) > 0)
   	  {
       		 // Process the record
        	 if(strcmp(faculty_detail.loginid,u.loginid)==0)
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
        		strcpy(faculty_detail.password,readBuffer);
    			faculty_detail.password[readBytes-1] = '\0';
    			bzero(writeBuffer, sizeof(writeBuffer));
			sprintf(writeBuffer, "\n%s%s","Changed Password=", faculty_detail.password);
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	
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
    			 strcpy(writeBuffer,"\nSuccessfully Updated\n");
			 writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			 return 1;
		  }
   	 }
   	 return 0;
   }
