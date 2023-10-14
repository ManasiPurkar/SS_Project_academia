/*Name- Manasi Purkar
Roll- MT2023158
File- Rand generate password
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LENGTH 9
// Function to generate a random password
char* generateRandomPassword() {
    // Define the character set from which to generate the password
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+";

    // Determine the size of the character set
    int charsetSize = sizeof(charset) - 1;

    // Seed the random number generator (initialize it only once)
    static int initialized = 0;
    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    // Allocate memory for the password
    char* password = (char*)malloc((LENGTH + 1) * sizeof(char));

    // Generate the password
    for (int i = 0; i < LENGTH; i++) {
        int randomIndex = rand() % charsetSize;
        password[i] = charset[randomIndex];
    }

    // Null-terminate the password string
    password[LENGTH] = '\0';

    return password;
}



