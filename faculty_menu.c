#include <stdio.h>
#include <unistd.h>

#include "login.h"
#include "faculty.h"


int faculty_menu()
{
	printf("______Welcome Faculty_______\n");
	printf("Choose one from below\n");
	printf("1.View offering courses\n 2.Add new course\n 3.Remove course\n 4.Update course\n 5.Logout\n");
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
			view_offering_courses();
			break;
		case 2:
			add_new_course();
			break;
		case 3:
			remove_course();
			break;
		case 4:
			update_course();
			break;
		case 5:
			logout()
			break;
		default:
			return 0;
	}
	return 1;
	
}

