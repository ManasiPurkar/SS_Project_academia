/*Name- Manasi Purkar
Roll- MT2023158
File- Faculty menu
*/
#include <stdio.h>
#include <unistd.h>

//#include "login.h"
#include "faculty.h"


int faculty_menu(int desc)
{
	while(1)
	{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
	strcpy(writeBuffer,"______Welcome Faculty_______\n");
	strcat(writeBuffer,"Choose one from below\n");
	strcat(writeBuffer,"1.View offering courses\n 2.Add new course\n 3.Remove course\n 4.Update course\n 5.Change Password \n 6.Logout\n");
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
			if(!view_offering_courses(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in showing offering courses\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 2:
			if(!add_new_course(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in adding new course\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 3:
			if(!remove_course(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in removing course\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 4:
			if(!update_course(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in updating course\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 5:
			if(!change_password(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in changing password\n");
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

