#include <stdio.h>
#include <string.h>



int main(){


    printf("type command line stuff\n");
    char c[2];
    scanf("%s",c);
    
    char parameter1[10];
    scanf("%s",parameter1);

    char parameter2[40];
    scanf("%s",parameter2);

    printf("The first parameter: %s\n", c);
    printf("The second parameter: %s\n", parameter1);
    printf("The third parameter: %s\n", parameter2);

    return 0;
}