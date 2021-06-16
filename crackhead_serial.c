#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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
    printf("Runnuing ->readFile...\n\n");

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
        while((ch = fgetc(fp)) != EOF){
            toReturn[c++] = ch;
                // reads text until newline is encountered
        }
    }

    fclose(fp);
    return toReturn;
}

/* Function to swap values at two pointers */
void swap(char* x, char* y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(char* a, int l, int r, char* password)
{
    int i;
    if (l == r){

        if (strlen(a) < 8) {
            // a random string?
            char salt[] = "$6$NGWhf/sJZ2Jgxbch$";
            char *encrypted = crypt(a, salt);

            printf("Ans: %s, Encrypted: %s\n", a, encrypted);
            if (strcmp(password, encrypted) == 0){
                printf("FOUND IT\n");
                exit(0);
            }
        }
        // sleep(3);
    }
    else {
        for (i = l; i <= r; i++) {
            swap((a + l), (a + i));
            permute(a, l + 1, r, password);
            swap((a + l), (a + i)); // backtrack
        }
    }
}


void crack_password(char* password){
    // generate alphabets array
    char alphabets[27];
    for(int i = 0; i < 26; i++){
        alphabets[i] = 97 + i;
    }
    alphabets[26] = '\0';

    permute(alphabets, 0, 26, password);
}


char* extractHashedPassword(char* user_name){
    printf("Running ->extractHasedPassword...\n");
    // read all file data
    char* file_details = readFile("/etc/shadow");
    
    // printf("%s", file_details);
    char* name_array = (char*)malloc(256*sizeof(char));
    char* password = (char*)malloc(256*sizeof(char));
    int temp_counter = 0; int file_counter = 0; 


    char* token = strtok(file_details, "\n");
    while (token) {
        token = token + '\0';
        int cc = 0;
        
        // printf("%s-%s-%i-%li-%li\n", token, "zohair", strcmp("\nzohair", token), strlen(token), strlen("\nzohair"));
        if (strcmp(user_name, token) == 0){
            token = strtok(NULL, ":");
            // printf("Password Mastiii: %s\n", token);
            return token;
        }
        token = strtok(NULL, ":");
    }
}


// usename: whisker
// password: helloworl

int main(void)
{
    // a password string we'll randomly guess
    char id[] = "abcdefghijklmnopqrstuvwxyz";
    char* user_name = (char*)malloc(25*sizeof(char));

    // a random string?
    char salt[] = "$6$NGWhf/sJZ2Jgxbch$";
    char *encrypted = crypt(id, salt);
    // printf("%s\n",encrypted);

    // input username
    printf("Enter username\n");
    fgets(user_name, 25, stdin);

    // add '\n' to user name for string matching (CHAPPI)
    char temp;
    for(int i = strlen(user_name) - 1; i > -1; i--){
        user_name[i + 1] = user_name[i];
    }
    user_name[0] = '\n';
    user_name[strlen(user_name) - 1] = '\0';

    // get hased password from file
    char* password = (extractHashedPassword(user_name));
    printf("Password Mastiii: %s\n<---------------------------------------------------->\n", password);

    if (password == NULL) {
        printf("Error: Username \"%s\" not found.\n", user_name);
        return(-1);
    }

    // get all permutations of alphabets
    crack_password(password);
}

