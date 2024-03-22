#include <stdio.h>
#include <string.h>


void swap (int* x, int* y){
    int z;
    z = *x;
    *x = *y;
    *y = z;
}
int main(){


    int a = 10, b = 20;

    swap(&a,&b);

    printf("a %d, b %d\n",a,b);

    // printf("type command line stuff\n");
    // char c[2];
    // scanf("%s",c);
    
    // int ID;
    // scanf("%d",&ID);

    // int prio;
    // scanf("%d",&prio);

    // printf("The first parameter: %s\n", c);
    // printf("The second parameter: %d\n", ID);
    // printf("The third parameter: %d\n", prio);



    return 0;
}