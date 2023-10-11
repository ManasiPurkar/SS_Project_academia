#include <stdio.h>
#include <unistd.h>

#include "login.h"
#include "admin.h"

int admin_menu()
{
	printf("______Welcome Admin_______\n");
	printf("Choose one from below\n");
	printf("1.Add student\n 2.View student\n 3.Add faculty\n 4.View faculty\n 5.Modify student\n 6.Modify faculty\n 7.Logout\n");
	printf("Enter Option number: \n");
    	char option[10];
    	int opt;
   	 if (read(STDIN_FILENO, option, sizeof(option)) == -1) {
       		 perror("Error in reading option");
        	return 0;
    	}
	opt = atoi(option);
	switch(opt)
	{
		case 1:
			addStudent();
			break;
		case 2:
			viewStudent();
			break;
		case 3:
			addfaculty();
			break;
		case 4:
			viewFaculty();
			break;
		case 5:
			modifyStudent();
			break;
		case 6:
			modifyFaculty();
			break;
		case 7:
			logout();
			break;
		default:
			return 0;
	}
	return 1;
	
}

