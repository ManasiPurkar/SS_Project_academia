#include <stdio.h>
#include <unistd.h>

#include "login.h"
#include "student.h"



int student_menu()
{
	printf("______Welcome Student_______\n");
	printf("Choose one from below\n");
	printf("1.View all courses\n 2.Enroll new course\n 3.Unenroll course\n 4.View enrolled courses\n 5.Logout\n");
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
			view_courses()
			break;
		case 2:
			enroll_course();
			break;
		case 3:
			unenroll_course();
			break;
		case 4:
			view_enrolled_courses();
			break;
		case 5:
			logout()
			break;
		default:
			return 0;
	}
	return 1;
	
}

