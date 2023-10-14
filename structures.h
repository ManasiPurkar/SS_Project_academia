/*Name- Manasi Purkar
Roll- MT2023158
File- All structs
*/

struct admin
{
	
	char loginid[5];
	char password[9];
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
        int no_of_offered_c;
        int offered_courses[10];

};
struct course
{
	int id;
	char name[20];
	char dept[10];
	int total_seats;
	int enrolled_seats;
	char offered_by[11];
	int enrolled_stud[200]; //enrolled seats can be max 200
	int credits;
};

