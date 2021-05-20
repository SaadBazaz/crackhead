#include <stdio.h>
#include <unistd.h>
#ifdef __linux__ 
    //linux code goes here
    #include <crypt.h>
// #elif _WIN32
    // windows code goes here
// #else
    // macOS code goes here

#endif
int main(void)
{
    // a password string we'll randomly guess
    char id[] = "mpiuser";

    // a random string?
    char salt[] = "$6$4GfdWqHx$";
    char *encrypted = crypt(id, salt);
    printf("%s\n",encrypted);
}