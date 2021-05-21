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


char* readFile(char* filename){
   char ch, file_name[25];
   FILE *fp;

   printf("Enter name of a file you wish to see\n");
   gets(file_name);

   fp = fopen(file_name, "r"); // read mode

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   printf("The contents of %s file are:\n", file_name);

   while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);

   fclose(fp);
}


char* extractHashedPassword(){

}


int main(void)
{
    // a password string we'll randomly guess
    char id[] = "mpiuser";

    // a random string?
    char salt[] = "$6$4GfdWqHx$";
    char *encrypted = crypt(id, salt);
    printf("%s\n",encrypted);

    // input username

    // get hased password from file
}