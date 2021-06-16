#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#include <omp.h>
#include <mpi.h>

#ifdef __linux__ 
    //linux code goes here
    #include <crypt.h>
// #elif _WIN32
    // windows code goes here
// #else
    // macOS code goes here
#endif
#define FILE_PATH = "etc/shadows"



// ---------------- Helper Functions ----------------

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


char* get_password_salt(char* password){
    char* temp = (char*)malloc(100*sizeof(char));
    strcpy(temp, password);
    char* toReturn = (char*)malloc(50*sizeof(char));
    toReturn[0] = '$';

    // tokenize words wrt '$'
    char* first = strtok(temp, "$");
    char* second = strtok(NULL, "$");

    // conactinate inorder to form proper salt
    strcat(toReturn, first);
    strcat(toReturn, "$");
    strcat(toReturn, second);
    strcat(toReturn, "$");

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

        printf("%s\n", a);
        // sleep(2);
        if (strlen(a) < 8) {
            // char *salt = (char*)malloc(50*sizeof(char));
            char* salt = get_password_salt(password);
            char* encrypted = crypt(a, salt);

            printf("Ans: %s, Encrypted: %s, with Salt: %s\n", a, encrypted, salt);
            if (strcmp(password, encrypted) == 0){
                printf("FOUND IT\n");
                exit(0);
            }
        }
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
    char alphabets[9];
    for(int i = 0; i < 8; i++){
        // alphabets[i] = 97 + i;
        alphabets[i] = 97;
    }
    alphabets[8] = '\0';

    // generate all permuatations
    permute(alphabets, 0, 8 , password);
}


char* extractHashedPassword(char* user_name){
    printf("Running ->extractHasedPassword...\n");
    // read all file data
    char* file_details = readFile("/etc/shadow");
    
    // initalize variables
    char* name_array = (char*)malloc(256*sizeof(char));
    char* password = (char*)malloc(256*sizeof(char));
    int temp_counter = 0; int file_counter = 0; 

    // tokenize file wrt to lines
    char* token = strtok(file_details, "\n");
    while (token) {
        token = token + '\0';
        int cc = 0;
        
        // extract password from next token
        if (strcmp(user_name, token) == 0){
            token = strtok(NULL, ":");
            return token;
        }
        token = strtok(NULL, ":");
    }
    return "";
}


int check_password(char* asli_pass, char* nakli_pass, char* salt){
    char* encrypted = crypt(nakli_pass, salt);
    printf("Real: %s | %li\nCalc: %s | %li\nPassword: %s | %li\nSalt: %s | %li\n\n", asli_pass, strlen(asli_pass), encrypted, strlen(encrypted), salt, strlen(salt), nakli_pass, strlen(nakli_pass));

    if (strcmp(asli_pass, encrypted) == 0){
        printf("CRACKED....\nPassword is: %s\n", nakli_pass);
        int toReturn = 1;
        // MPI_Send(&toReturn, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
        MPI_Abort(MPI_COMM_WORLD, -1);
        exit(1);
    }
    return 0;
}


// To hold the status of MPI_Recv
MPI_Status status;

int main(int argc, char** argv) {

    // char id[] = "abcdefghijklmnopqrstuvwxyz";
    // char salt[] = "$6$NGWhf/sJZ2Jgxbch$";
    // char *encrypted = crypt(id, salt);

    int dividedArraySize, start_offset = 0, end_offset = 0, a_size = 0;
    int* shugal = (int*)malloc(1*sizeof(int));
    char* user_name = (char*)malloc(25*sizeof(char));
    char* password = (char*)malloc(256*sizeof(char));
    char* salt = (char*)malloc(50*sizeof(char));
    char *password_combinations = (char*)malloc(8*sizeof(char));

    int myrank, nprocs, nworkers, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    nworkers = nprocs - 1;

    // exit if number of processes is less than 2
    if (nprocs <= 1) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
        exit(1);
    }

    // MASTER PROCESS
    if (myrank == 0) {
        printf("\n---------------------------------\nAuthors:\n*Zohair Hadi, 18i-0671*\n*Mahrukh Waqar, 18i-0000*\n*Abdurehman Subahni, 18i-0000*\n*Saad Ahmed, 18i-0000*\n---------------------------------\n\n");

        // input username
        printf("Enter User Name: ");
        fgets(user_name, 25, stdin);

        // add '\n' to user name for string matching (CHAPPI)
        char temp;
        for(int i = strlen(user_name) - 1; i > -1; i--){
            user_name[i + 1] = user_name[i];
        }
        user_name[0] = '\n';
        user_name[strlen(user_name) - 1] = '\0';

        // get hased password from file
        password = extractHashedPassword(user_name);
        password[strlen(password)] = '\0';
        // get salt
        salt = get_password_salt(password);
        printf("Password Mastiii: %s\n<-------------------------------------------------------------------->\n", password);

        // Exit if username not found in the shadows file
        if (password == NULL) {
            printf("Error: Username not found.\n\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
            exit(1);
        }

        a_size = 26 / nworkers;
        start_offset = 0;
        for(int i=1; i <= nworkers; i++){
            MPI_Send(password, 256, MPI_CHAR, i, 1, MPI_COMM_WORLD);
            MPI_Send(salt, 50, MPI_CHAR, i, 1, MPI_COMM_WORLD);
            MPI_Send(&start_offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&a_size, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            start_offset += a_size;
        }

        // // get all permutations of alphabets
        // crack_password(password);
    }

    // SLAVE PROCESSES 
    else {
        // recieve the array and data
        char* temp_password = (char*)malloc(256*sizeof(char));
        char* temp_salt = (char*)malloc(50*sizeof(char));
        MPI_Recv(temp_password, 256, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(temp_salt, 50, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&start_offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&a_size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        // // printf("Rank: %i with %s\n", myrank, password_combinations);
        // #pragma omp parallel num_threads(2) 
        // {
        //     if (omp_get_thread_num() == 1){
                for (int a = start_offset; a < start_offset + a_size; a++){
                    // printf("%c\n", a + 97);
                    
                    password_combinations[0] = a + 97;
                    password_combinations[1] = '\0';
                    check_password(temp_password, password_combinations, temp_salt);

                    for(int b=0; b < 26; b++){
                        // printf("%c%c\n", a + 97, b + 97);
                        
                        password_combinations[0] = a + 97;
                        password_combinations[1] = b + 97;
                        password_combinations[2] = '\0';
                        check_password(temp_password, password_combinations, temp_salt);

                        for(int c=0; c < 26; c++){
                            // printf("%c%c%c\n", a + 97, b + 97, c + 97);
                            password_combinations[0] = a + 97;
                            password_combinations[1] = b + 97;
                            password_combinations[2] = c + 97;
                            password_combinations[3] = '\0';
                            check_password(temp_password, password_combinations, temp_salt);
                            
                            for(int d=0; d < 26; d++){
                                    // printf("%c%c%c%c\n", a + 97, b + 97, c + 97, d + 97);
                                    password_combinations[0] = a + 97;
                                    password_combinations[1] = b + 97;
                                    password_combinations[2] = c + 97;
                                    password_combinations[3] = d + 97;
                                    password_combinations[4] = '\0';
                                    check_password(temp_password, password_combinations, temp_salt);
                                    
                                for(int e=0; e < 26; e++){
                                        // printf("%c%c%c%c%c\n", a + 97, b + 97, c + 97, d + 97, e + 97);
                                        password_combinations[0] = a + 97;
                                        password_combinations[1] = b + 97;
                                        password_combinations[2] = c + 97;
                                        password_combinations[3] = d + 97;
                                        password_combinations[4] = e + 97;
                                        password_combinations[5] = '\0';
                                        check_password(temp_password, password_combinations, temp_salt);
                                        
                                    for(int f=0; f < 26; f++){
                                            // printf("%c%c%c%c%c%c\n", a + 97, b + 97, c + 97, d + 97, e + 97, f + 97);
                                            password_combinations[0] = a + 97;
                                            password_combinations[1] = b + 97;
                                            password_combinations[2] = c + 97;
                                            password_combinations[3] = d + 97;
                                            password_combinations[4] = e + 97;
                                            password_combinations[5] = f + 97;
                                            password_combinations[6] = '\0';
                                            check_password(temp_password, password_combinations, temp_salt);

                                        for(int g=0; g < 26; g++){
                                                // printf("%c%c%c%c%c%c%c\n", a + 97, b + 97, c + 97, d + 97, e + 97, f + 97, g + 97);
                                                password_combinations[0] = a + 97;
                                                password_combinations[1] = b + 97;
                                                password_combinations[2] = c + 97;
                                                password_combinations[3] = d + 97;
                                                password_combinations[4] = e + 97;
                                                password_combinations[5] = f + 97;
                                                password_combinations[6] = g + 97;
                                                password_combinations[7] = '\0';
                                                check_password(temp_password, password_combinations, temp_salt);

                                            for(int h=0; h < 26; h++){
                                                // printf("%c%c%c%c%c%c%c%c\n", a + 97, b + 97, c + 97, d + 97, e + 97, f + 97, g + 97, h + 97);
                                                password_combinations[0] = a + 97;
                                                password_combinations[1] = b + 97;
                                                password_combinations[2] = c + 97;
                                                password_combinations[3] = d + 97;
                                                password_combinations[4] = e + 97;
                                                password_combinations[5] = f + 97;
                                                password_combinations[6] = g + 97;
                                                password_combinations[7] = h + 97;
                                                password_combinations[8] = '\0';

                                                check_password(temp_password, password_combinations, temp_salt);
                                                // sleep(1);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // int toReturn = -1;
                // MPI_Send(&toReturn, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
            // }

            // else {
            //     int abortSearch = 0;
            //     // recieve abort message from master and exit
            //     MPI_Recv(&abortSearch, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, &status);
            //     if (abortSearch == 1) {
            //         printf("Process %i is ABORTING\n", myrank);
            //         exit(1);
            //     }
            // }
        // }
    }

//     // We are in the Master process
//     if (myrank == 0){

//         // Initialize search array in master process
// 	const int search_array_length = MAX_LENGTH;
//         int search_array [MAX_LENGTH];

// 	srand(time(NULL));

// 	for (i=0; i<search_array_length; i++){
// 		search_array[i] = 2 + rand();
// 	}

// 	// Set the last value to 1, so we have a good enough challenge ;)
// 	search_array[search_array_length-1] = 1;


// 	// Calculate how much each process will receive
// 	int slice_size = search_array_length/(nprocs - 1);
// 	int remainder = search_array_length%(nprocs - 1);

// 	omp_set_num_threads(2);

// 	#pragma omp parallel num_threads(2) firstprivate(search_array)
// 	{
// 		int id = omp_get_thread_num();

// 		if (id == 0){

// 			//printf ("Main array is\n");
// 			//int j=0;
// 			//printf("[");
// 			//for (j=0; j<100; j++){
// 			//	printf("%d, ", search_array[j]);
// 			//}
// 			//printf("]");


// 			// Send a slice of the search_array to each process

// 			MPI_Status status_1;
// 			int offset = 0;
// 			int curr_slice_size = slice_size;
// 			for (i=1; i<nprocs; i++){
// 			    int buf2;
// 			    // Tag 5 is for READY
// 			    MPI_Recv(&buf2, 1, MPI_INT, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, &status_1);
// 			    printf ("MASTER: Received ready signal from PROCESS %d.\n", status_1.MPI_SOURCE);
// 			    curr_slice_size = slice_size;
// 			    if (status_1.MPI_SOURCE == (nprocs - 1) ){
// 				curr_slice_size += remainder;
// 			    }

// 			    offset = (status_1.MPI_SOURCE-1)*slice_size;
// 			    // Tag 2 is for DISPATCH
// 			    MPI_Send(&search_array[offset], curr_slice_size, MPI_INT, status_1.MPI_SOURCE, 2, MPI_COMM_WORLD);
// 			    printf ("MASTER: Slice sent to PROCESS %d, offset = %d.\n", status_1.MPI_SOURCE, offset);
// 			}

// 		}
// 		else if (id == 1){
// 			// Listen to response from any process
// 			MPI_Status status_2;
// 			int p_counter = 0;

// 			while (p_counter != (nprocs-1) ){

// 			    int ret;
// 			    // Tag 3 is for RESULT
// 			    MPI_Recv(&ret, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status_2);
// 			    printf("MASTER: Received a response from PROCESS %d, returned = %d!\n", status_2.MPI_SOURCE, ret);
// 			    if (ret > -1){
// 				ret = (status_2.MPI_SOURCE-1)*slice_size + ret;
// 				printf("\n------- Result has been found! Array[%d] == %d -------\n\n", ret, search_term);
// 				printf("MASTER: Sending kill signal to all processes...\n");
// 				for (i=1; i<nprocs; i++){
// 					// Tag 8 is for KILL
// 					MPI_Send(&i, 1, MPI_INT, i, 8, MPI_COMM_WORLD);
// 				}
// 				printf("\nThis program was made by Saad Bazaz, i180621.\n");
// 				break;
// 			    }
			    
// 			    p_counter++;
// 			}

// 			if (p_counter == (nprocs - 1))
// 				printf("\nCould not find result.\n");
// 		}
// 	}


//     }
//     else { // Slave process

//         int child_array[MAX_LENGTH];

//         printf("\tPROCESS %d: Sending ready signal to master...\n", myrank);
// 	int ready = 1;
// 	// If ready, send 1 on Tag 5
//         MPI_Send(&ready, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);

//         // Receive array from parent
//         printf("\tPROCESS %d: Waiting for array from master...\n", myrank);
//         MPI_Recv(&child_array, MAX_LENGTH, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);		
//         int child_array_length;
//         MPI_Get_count(&status, MPI_INT, &child_array_length);
//         printf("\tPROCESS %d: Received array of size %d.\n", myrank, child_array_length);
  	
// 	int maxthreads = 2;

// 	//printf("Maxthreads are %d\n", maxthreads);

// 	omp_set_num_threads(maxthreads);

// 	// Equally divide the array amongst the threads
// 	int slice_size = child_array_length;
// 	//int slice_size = child_array_length / maxthreads;
// 	//int remainder = child_array_length % maxthreads;

// 	//bool* is_complete;

// 	#pragma omp parallel num_threads(maxthreads) firstprivate(child_array)
// 	{
// 		int id = omp_get_thread_num();

// 		if (id == 0){ // in First Thread
// 			int get_kill = 0;
// 			// Listen for KILL signal from parent
// 			MPI_Recv(&get_kill, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, &status);	
// 			printf("\t\tPROCESS %d THREAD %d: Committing lifen't.\n", myrank, id);	
// 			exit(0);
// 		}

// 		else {	// Else in other threads
// 			// Search for the number
// 			printf("\t\tPROCESS %d THREAD %d: Searching in array...\n", myrank, id);
// 			//int index = -1;			
// 			int curr_slice_size = slice_size;
// 			//if ( id == (maxthreads - 1) ){
// 			//curr_slice_size += remainder;		
// 			//}
// 			int index = search(&child_array[(id-1)*curr_slice_size], curr_slice_size, search_term);
// 			printf("\t\tPROCESS %d THREAD %d: Search complete. Result is %d.\n", myrank, id, index);
			

// 			// Send index to master process
// 			printf("\t\tPROCESS %d THREAD %d: Sending result to master...\n", myrank, id);
// 			MPI_Send(&index, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
// 		}
// 	}
//    }

// End
MPI_Finalize();

}

