

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
	char loginid[]="ad01";
	char password[]="sadmin123";
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
        int offered__courses[10];

};
struct course
{
	char name[20];
	char dept[10];
	int total_seats;
	int enrolled_seats;
	int credits;
};

