#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>

#include "login.c"

//PHD_2 : _42ckV*@q
int view_courses();
int enroll_course();
int unenroll_course();
int unenroll(int stdid,int cid);
int main()
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
	if(!view_courses())
	{
		printf("error in view offering courses\n");
	}
	/*if(!add_new_course())
	{
		printf("error in adding courses\n");
	}
	if(!update_course())
	{
		printf("error in updating courses\n");
	}*/	
	return 0;
}
int view_courses()
{
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
       		printf("id= %d\n",course_detail.id);
       		printf("name= %s\n",course_detail.name);
       		printf("dept= %s\n",course_detail.dept);
       		printf("total_seats= %d\n",course_detail.total_seats);
       		printf("enrolled_seats= %d\n",course_detail.enrolled_seats);
       		printf("credits= %d\n",course_detail.credits);
       		printf("offered by= %s\n",course_detail.offered_by);
       		for(int i=0;i<course_detail.enrolled_seats;i++) 
       		{
       		printf("enrolled stud ids= %d ",course_detail.enrolled_stud[i]);
       		}
       		printf("\n");
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

int enroll_course()
{	
	int cid;
	printf("Enter course id which you want to enroll= \n");
	
	char reqcid[10];
   	 if (read(STDIN_FILENO, reqcid, sizeof(reqcid)) == -1) {
       		 perror("Error in reading course id");
        	return 0;
    	}
	cid = atoi(reqcid);
	
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
			/*char delimiter[] = "_";
			char *token = strtok(u.loginid, delimiter); //to get student id from its login id
    			while (token != NULL) {
      			  // Move to the next token
      			  token = strtok(NULL, delimiter);
       			 if (token != NULL) {
         		   		stdid=atoi(token);
        			}
  			  }*/
  			int empty=-1;
  			for(int i=0;i<course_detail.enrolled_seats;i++)
  			{
  				if(course_detail.enrolled_stud[i]==stdid)
  				{
  					printf("course already enrolled\n");
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

int unenroll_course()
{
	int cid;
	printf("Enter course id which you want to unenroll= \n");
	
	char reqcid[10];
   	 if (read(STDIN_FILENO, reqcid, sizeof(reqcid)) == -1) {
       		 perror("Error in reading course id");
        	return 0;
    	}
	cid = atoi(reqcid);
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
			for(int i=0;i<course_detail.enrolled_seats;i++)
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
