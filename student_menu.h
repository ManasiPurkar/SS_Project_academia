/*Name- Manasi Purkar
Roll- MT2023158
File- Student menu
*/

#include <stdio.h>
#include <unistd.h>




int student_menu(int desc)
{
	while(1)
	{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
	strcpy(writeBuffer,"______Welcome Student_______\n");
	strcat(writeBuffer,"Choose one from below\n");
	strcat(writeBuffer,"1.View all courses\n 2.Enroll new course\n 3.Unenroll course\n 4.View enrolled courses\n 5.Change Passsword\n 6.Logout\n");
	strcat(writeBuffer,"Enter Option number: \n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
                perror("Error while writing options to client!");
                return 0;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
    	/*char option[10];
    	int opt;
   	 if (read(STDIN_FILENO, option, sizeof(option)) == -1) {
       		 perror("Error in reading option");
        	return 0;
    	}
	opt = atoi(option);*/
	readBytes = read(desc, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
         {
                perror("Error while reading client's choice for ADMIN_MENU");
                return 0;
        }
        int opt=atoi(readBuffer);
        int authenticated=1;
	switch(opt)
	{
		case 1:
			if(!view_courses(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in view courses\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 2:
			if(!enroll_course(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in enroll course\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 3:
			if(!unenroll_course(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in unenroll\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 4:
			if(!view_enrolled_courses(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in view enrolled courses\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 5:
			if(!change_password_stud(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in change password\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 6:
			authenticated=0;
			break;
		default:
			strcat(writeBuffer,"Wrong choice\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			return 0;
	}
	if(authenticated==0)
	{
	bzero(writeBuffer, sizeof(writeBuffer));
	strcat(writeBuffer,"Successfully logout\n");
	writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
	break;
	}
	}	
	return 1;
	
}

