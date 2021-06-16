// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// void swap(char **s, int i, int j)
// {
//     char * tmp = s[i];
//     s[i] = s[j];
//     s[j] = tmp;
// }

// void reverse(char **s, int start, int end)
// {
//     while(start<end)
//     {
//         swap(s,start++,end--);
//     }
// }

// int next_permutation(int n, char **s)
// {
// 	/**
// 	* Complete this method
// 	* Return 0 when there is no next permutation and 1 otherwise
// 	* Modify array s to its next permutation
// 	*/
//     for(int i=n-2;i>-1;i--)
//     {
//         if(strcmp(s[i+1],s[i])>0)
//         {
//             //get min max
//             for(int j=n-1;j>i;j--)
//             {
//                 if(strcmp(s[j],s[i])>0)
//                 {
//                     //do swap
//                     swap(s,i,j);
//                     // do reverse
//                     reverse(s,i+1,n-1);
//                     return 1;
//                 }
//             }
//         }
//     }
//     return 0;
// }

// int main()
// {
// 	char **s; int n = 6;
//     char* temp = (char*)malloc(1*sizeof(char));
// 	s = calloc(n, sizeof(char*));

// 	for (int i = 0; i < n; i++){
// 		s[i] = calloc(11, sizeof(char));
//         temp[0] = 97 + i;
//         strcpy(s[i], temp);
// 	}

// 	do{
// 		for (int i = 0; i < n; i++)
// 			printf("%s", s[i]);
//         printf("\n");
// 	} while (next_permutation(n, s));

// 	for (int i = 0; i < n; i++)
// 		free(s[i]);
// 	free(s);
// 	return 0;
// }
// a



//     // *s = "abcdefghijklmnopqrstuvwxyz";


void perm(char a[], int level){

    static int flag[10] = {0};
    static char res[10];
    // If we are the last character of the input string 
    if(a[level] == '\0'){
        // First we assign stopping point to result
        res[level] = '\0';
        // Now we print everything
        for(int i = 0; res[i] != '\0'; ++i){
            printf("%c", res[i]);
        }
        printf("\n");
    }
    else{
        // Scan the original string and flag to see what letters are available
        for(int i = 0; a[i] != '\0'; ++i){
            if(flag[i] == 0){
                res[level] = a[i];
                flag[i] = 1;
                perm(a, level + 1);
                flag[i] = 0;
            }
        }
    }
}

int main(){
    char first[] = "abc";
    perm(first, 0);
    return 0;
}