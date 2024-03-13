#include <stdio.h>
#include <string.h>



int main(){


    printf("type command line stuff");
    char what[40];
    scanf(what)

    const char s[2] = " ";

    char* c, paramater1, paramater2;
    
    c = strtok(what, s);
    paramater1 = strtok(what, s);
    paramater2 = strtok(what, s);

    return 0;
}