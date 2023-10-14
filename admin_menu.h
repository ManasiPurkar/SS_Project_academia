/*Name- Manasi Purkar
Roll- MT2023158
File- Admin menu
*/
#include <stdio.h>
#include <unistd.h>

//#include "login.h"
#include "admin.h"


int admin_menu(int desc)
{
	while(1)
	{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
   	bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
	strcpy(writeBuffer,"______Welcome Admin_______\n");
	strcat(writeBuffer,"Choose one from below\n");
	strcat(writeBuffer,"1.Add student\n 2.View student\n 3.Add faculty\n 4.View faculty\n 5.Modify student\n 6.Modify faculty\n 7.Logout\n");
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
			if(!addStudent(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in add student\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 2:
			if(!viewStudent(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in view student\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 3:
			if(!addfaculty(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in add faculty\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 4:
			if(!viewFaculty(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in view faculty\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 5:
			if(!modifyStudent(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in modify student\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 6:
			if(!modifyFaculty(desc))
			{
			bzero(writeBuffer, sizeof(writeBuffer));
			strcat(writeBuffer,"Error in modify faculty\n");
			writeBytes = write(desc, writeBuffer, strlen(writeBuffer));
			}
			break;
		case 7:
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

