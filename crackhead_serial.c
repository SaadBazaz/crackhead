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


int cmp_str(char* array1, char* array2){
    printf("Comparing:%s-%s-%li-%li\n", array1, array2, strlen(array1), strlen(array2));
    int s = strlen(array1);
    for (int i=0; i < s; i++){
        printf("HELLL\n");
        printf("%i\n", array1[i] != '\n');
        if (array1[i] != '\n'){
            if (array1[i] != array2[i]){
                return 0;
            }
        }
    }
    return 1;
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
    // token[strlen(token)] = '\0';
        token = token + '\0';
        printf("%s-%s-%i-%li-%li\n", token, user_name, cmp_str(user_name, token), strlen(token), strlen(user_name));

        if (strncmp(user_name, token, 6) == 0){
            printf("Nigger");
        }

        // if (cmp_str(user_name, token)){
        //     token = strtok(NULL, ":");
        //     printf("Password Mastiii: %s\n", token);
        //     return token;
        // }
        token = strtok(NULL, ":");
    }


    // while (file_details[file_counter] != NULL) {
    //     temp_counter = 0;
    //     name_array = (char*)malloc(256*sizeof(char));

    //     name_array[temp_counter] = file_details[file_counter];
    //     temp_counter += 1;
    //     file_counter += 1;
    //     printf("%c", name_array[temp_counter-1]);
    //     if (name_array[temp_counter] == ':'){
    //         printf(name_array);
    //         name_array[5] = '\0';
    //         printf(name_array);

    //         int is_same = cmp_str(user_name, name_array);
    //         printf("SHUGAL MELA: %i\n", is_same);
    //         return NULL;

    //         if (is_same == 1){
    //             temp_counter = 0;
    //             file_counter += 1;
    //             while(file_details[file_counter] != ':')
    //                 password[temp_counter++] = file_details[file_counter++];
                
    //             return password;
    //         } 
    //         else temp_counter = 0;
    //     }
    // }
    // return NULL;
}


int main(void)
{
    // a password string we'll randomly guess
    char id[] = "mpiuser";
    char* user_name = (char*)malloc(25*sizeof(char));

    // a random string?
    char salt[] = "$6$4GfdWqHx$";
    char *encrypted = crypt(id, salt);
    // printf("%s\n",encrypted);

    // input username
    printf("Enter username\n");
    fgets(user_name, 25, stdin);


    // user_name[strlen(user_name) - 1] = '\0';
 
    // printf("", strlen(user_name))
 
    // printf("2. %s----%li\n", user_name, strlen(user_name));

    // get hased password from file
    (extractHashedPassword(user_name));
}