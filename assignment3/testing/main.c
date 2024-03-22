#include <stdio.h>
#include <string.h>



int main(){


    printf("type command line stuff\n");
    char c[2];
    scanf("%s",c);
    
    int ID;
    scanf("%d",&ID);

    int prio;
    scanf("%d",&prio);

    printf("The first parameter: %s\n", c);
    printf("The second parameter: %d\n", ID);
    printf("The third parameter: %d\n", prio);

    return 0;
}