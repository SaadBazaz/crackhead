#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef __linux__ 
    //linux code goes here
    #include <crypt.h>
// #elif _WIN32
    // windows code goes here
// #else
    // macOS code goes here
#endif
#define FILE_PATH = "etc/shadows"

char* readFile(char* filename){
    printf("Runnuing ->readFile...\n");

    char ch;
    char* toReturn = 0;
    FILE *fp;
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        perror("\nError while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // calculate total file length
    fseek (fp, 0, SEEK_END);
    long length = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    // make new string with file length
    toReturn = malloc (length);
    int c = 0;
    if (toReturn)
    {
        while((ch = fgetc(fp)) != EOF)
            toReturn[c++] = ch;
    }

    fclose(fp);
    return toReturn;
}


char* extractHashedPassword(char user_name[25]){
    printf("Runnuing ->extractHasedPassword...\n");
    // read all file data
    char* file_details = readFile("/etc/shadow");
    
    printf("%s", file_details);
    // loop over data
    // read line by line anc comapre

    // if line matches user_name, extract password
    // return hashed password

    // return null if not found
}


int main(void)
{
    // a password string we'll randomly guess
    char id[] = "mpiuser";
    char user_name[25];

    // a random string?
    char salt[] = "$6$4GfdWqHx$";
    char *encrypted = crypt(id, salt);
    // printf("%s\n",encrypted);

    // input username
    printf("Enter username\n");
    fgets(user_name, 25, stdin);

    // get hased password from file
    extractHashedPassword(user_name);
}