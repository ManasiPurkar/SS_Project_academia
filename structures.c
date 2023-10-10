#include<stdio.h>

/*struct superadmin
{
	char loginid[]="sad01";
	char password[]="sadmin123";
};*/
struct admin
{
	/*int id;
	char loginid[9];
	char password[10];
	*/
	char loginid[];
	char password[];
};
struct admin a={"Ad01","sadmin123"};
struct user
{
	char loginid[11]; 
        char password[10];
        char usertype; //A or F or S
};
struct student
{
	int id;
	char loginid[11]; //format CoursenameYearId
        char password[10];
	char course[5];
	char name[30];
	int age;
	char email[30];
	char address[100];
	char homestate[20];
	int mobno[10];
	int enrolled_courses[10];
};
struct faculty
{
        int id;
        char loginid[11]; //format FacId
        char password[10];
        char name[30];
        char email[30];
        int mobno[10];
	int offered_courses[20];
};
struct course
{
	int id;
	char name[20];
	char dept[10];
	int total_seats;
	int enrolled_seats;
	int offered_by;
	int enrolled_stud[enrolled_seats];
	int credits;
};

