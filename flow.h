/*#include "login.h"
#include "admin_menu.h"
#include "faculty_menu.h"
#include "faculty.h"
#include "admin.h"
#include "structures.h"
#include "student.h"
#include "common.h"*/

/*if(success)
{
if(u.usertype=='A')
    	{
    		int status=admin_menu(desc);
    		if(status==0)
    		{
    			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"Admin menu cannot be shown \n");
    			write(desc, writeBuffer, strlen(writeBuffer));
    		}
    	}
else if(u.usertype=='F')
    	{
    		int status=faculty_menu(desc);
    		if(status==0)
    		{
    			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"Faculty menu cannot be shown \n");
    			write(desc, writeBuffer, strlen(writeBuffer));
    		}
    	}
}*/
#include "structures.h"
#include <string.h>
struct user u;
void currentuser(struct user *up)
{
	strcpy(u.loginid,up->loginid);
	strcpy(u.password,up->password);
	u.usertype=up->usertype;
}
