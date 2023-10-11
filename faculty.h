#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>

//#include "login.c"
//#include "student.c" (check this at last)

//for testing Fac_4=v500TVuKW
int view_offering_courses();
int add_new_course();
int remove_course();
int update_course();

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
	if(!view_offering_courses())
	{
		printf("error in view offering courses\n");
	}
	if(!add_new_course())
	{
		printf("error in adding courses\n");
	}
	if(!update_course())
	{
		printf("error in updating courses\n");
	}	
	return 0;
}*/
int view_offering_courses()
{
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
			 printf("No. of offered courses= %d\n",faculty_detail.no_of_offered_c);
			 printf("Offered courses=");
			 for(int i=0;i<10;i++)
			 {
			    	printf("%d, ",faculty_detail.offered_courses[i]);
			 }
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

    	// Close the file
    	close(facfd);
    	return 0;  
}
int add_new_course()
{
	
	ssize_t readb;
	char readbuff[1000];
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
	//printf("%d",newc.id);
	printf("Enter course name: \n");
	ssize_t bytesRead = read(STDIN_FILENO, newc.name, sizeof(newc.name));
	if (bytesRead <= 1){
        perror("Error in storing name\n"); //equal to 1 for handling empty name
        return 0;
    	}
    	newc.name[bytesRead-1] = '\0';
    	printf("Enter department: \n");
    	bytesRead = read(STDIN_FILENO, newc.dept, sizeof(newc.dept));
	if (bytesRead <= 1){
        perror("Error in storing departmet\n");
        return 0;
    	} 
    	newc.dept[bytesRead-1] = '\0';
    	printf("Enter total seats: \n");
    	char total_seats[10];
   	 if (read(STDIN_FILENO, total_seats, sizeof(total_seats)) == -1) {
       		 perror("Error reading total_seats");
        	return 0;
    	}
	newc.total_seats = atoi(total_seats);
    	newc.enrolled_seats=0;
    	strcpy(newc.offered_by,u.loginid);
	for(int i=0;i<200;i++)
	{
	newc.enrolled_stud[i]=0;
	}
	printf("Enter credits: \n");
	char credits[10];
   	 if (read(STDIN_FILENO, credits, sizeof(credits)) == -1) {
       		 perror("Error reading credits");
        	return 0;
    	}
	newc.credits = atoi(credits);
	
	
	printf("id=%d \n",newc.id);
	printf("name=%s \n",newc.name);
    	printf("dept=%s \n",newc.dept);
    	printf("total_seats=%d \n",newc.total_seats);
    	printf("credits=%d \n",newc.credits);
    	printf("offered_by=%s \n",newc.offered_by);
	
	ssize_t bytesWrite=write(cfd,&newc,sizeof(struct course));
	if (bytesWrite <= 0){
        perror("Error in writing struct\n");
        return 0;
    	}
    	close(cfd);
    	
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
			for(int i=0;i<10;i++)
			{if(faculty_detail.offered_courses[i]==0)
			{
				faculty_detail.offered_courses[i]=newc.id;
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
    	  
			
	return 1;
}

int remove_course()
{
	int cid;
	printf("Enter course id which you want to remove= \n");
	
	char reqcid[10];
   	 if (read(STDIN_FILENO, reqcid, sizeof(reqcid)) == -1) {
       		 perror("Error reading course id");
        	return 0;
    	}
	cid = atoi(reqcid);
	
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
        return 1; 
        
   	} 
   	 return 0;
}
int update_course()
{
	int cid;
	printf("Enter course id which you want to update= \n");
	
	char reqcid[10];
   	 if (read(STDIN_FILENO, reqcid, sizeof(reqcid)) == -1) {
       		 perror("Error reading course id");
        	return 0;
    	}
	cid = atoi(reqcid);
	
	ssize_t bytesRead;
	printf("reqc=%d\n",cid);
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
		printf("Enter course name: \n");
		bytesRead = read(STDIN_FILENO, course_detail.name, sizeof(course_detail.name));
		if (bytesRead <= 1){
        		perror("Error in storing name\n"); //equal to 1 for handling empty name
        		return 0;
    		}
    		course_detail.name[bytesRead-1] = '\0';
    		printf("Enter department: \n");
    		bytesRead = read(STDIN_FILENO, course_detail.dept, sizeof(course_detail.dept));
		if (bytesRead <= 1){
        	perror("Error in storing departmet\n");
        	return 0;
    		} 
    		course_detail.dept[bytesRead-1] = '\0';
    		printf("Enter credits: \n");
    		char credit[10];
    		printf("Decrease total seats by: \n");
    		if (read(STDIN_FILENO, credit, sizeof(credit)) == -1) {
       		 perror("Error reading credits");
        	return 0;
    		}
    		course_detail.credits = atoi(credit);
    		
    		int dec_seats;
    		char decseats[10];
    		printf("Decrease total seats by: \n");
    		if (read(STDIN_FILENO, decseats, sizeof(decseats)) == -1) {
       		 perror("Error reading decseats");
        	return 0;
    		}
    		dec_seats = atoi(decseats);
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
    		char incseats[10];
    		printf("Increase total seats by: \n");
    		if (read(STDIN_FILENO, incseats, sizeof(incseats)) == -1) {
       		 perror("Error reading incseats");
        	return 0;
    		}
    		inc_seats = atoi(incseats);
    		course_detail.total_seats+=inc_seats;
    		ssize_t bytesWrite=write(cfd,&course_detail,sizeof(struct course));
		if (bytesWrite <= 0){
       			 perror("Error in writing struct\n");
      			  return 0;
    		}
		writel.l_type=F_UNLCK;
		fcntl(cfd, F_SETLKW, &writel);
		close(cfd);
		return 1;
	}
	else
	{
	 printf("course not found\n");
	 return 0;
	}
	return 0;	
}
