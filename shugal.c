#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stddef.h>

void swap(char **s, int i, int j)
{
    char * tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
}

void reverse(char **s, int start, int end)
{
    while(start<end)
    {
        swap(s,start++,end--);
    }
}

int next_permutation(int n, char **s)
{
	/**
	* Complete this method
	* Return 0 when there is no next permutation and 1 otherwise
	* Modify array s to its next permutation
	*/
    for(int i=n-2;i>-1;i--)
    {
        if(strcmp(s[i+1],s[i])>0)
        {
            //get min max
            for(int j=n-1;j>i;j--)
            {
                if(strcmp(s[j],s[i])>0)
                {
                    //do swap
                    swap(s,i,j);
                    // do reverse
                    reverse(s,i+1,n-1);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int main()
{
	char **s; int n = 4;
    char* temp = (char*)malloc(1*sizeof(char));
	s = calloc(n, sizeof(char*));

    s[0] = calloc(11, sizeof(char));
    temp[0] = 0;
    strcpy(s[0], temp);
	for (int i = 1; i < n; i++){
		s[i] = calloc(11, sizeof(char));
        temp[0] = 97 + i - 1;
        strcpy(s[i], temp);
	}

	do{
		for (int i = 0; i < n; i++)
			printf("%s", s[i]);
        printf("\n");
	} while (next_permutation(n, s));

	for (int i = 0; i < n; i++)
		free(s[i]);
	free(s);
	return 0;
}




    // *s = "abcdefghijklmnopqrstuvwxyz";



// int shugal(int array[], size_t length){
// 	// Find non-increasing suffix
// 	if (length == 0)
// 		return 0;
// 	size_t i = length - 1;
// 	while (i > 0 && array[i - 1] >= array[i])
// 		i--;
// 	if (i == 0)
// 		return 0;

//     // Find successor to pivot
// 	size_t j = length - 1;
// 	while (array[j] <= array[i - 1])
// 		j--;
// 	int temp = array[i - 1];
// 	array[i - 1] = array[j];
// 	array[j] = temp;

//     // Reverse suffix
// 	j = length - 1;
// 	while (i < j) {
// 		temp = array[i];
// 		array[i] = array[j];
// 		array[j] = temp;
// 		i++;
// 		j--;
// 	}
// 	return 1;
// }


// int main(){
//     int array[] = ;
//     do 
//     {
//         for(int i = 0; i < 5; i++){
//             printf("%i, ", array[i]);
//         }
//         printf("\n");
//     } while (shugal(array, 5) == 1);
// }