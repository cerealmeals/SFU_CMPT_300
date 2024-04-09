#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "functions.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[]) {
    bool flag_i = false;
    bool flag_l = false;
    int arg1_len = 0;
    int arg2_len = 0;

    char* str1;    
    char* str2;

    char path[PATH_MAX] = "\0";
    
    char ch = '-';                                                                                                                                              
    char *ret1;
    char *ret2;
    printf("argc #: %d\n", argc);

    if(argc == 3){                                                                                                                                          
        arg1_len = strlen(argv[2]);
        str1 = argv[2];
        ret1 = strchr(str1, ch);
        printf("String after |%c| is: |%s|\n", ch, ret1);

        printf("length of arg1: %d\n", arg1_len);

        if (arg1_len == 2)
        {
            if (ret1[1] == 'i')
            {
                flag_i = true;
                printf("flag i is true\n");
            }
            else if (ret1[1] == 'l')
            {
                flag_l = true;
                printf("flag l is true\n");
            }
            else
            {
                printf("Not the correct arguments, try 'i' or 'l'\n");
            }
        }       
        else if (arg1_len == 3)
        {
            if (ret1[1] == 'i' && ret1[2] == 'l')
            {
                flag_i = true;
                flag_l = true;
                printf("flag i and l is true\n");
            }
            else if (ret1[1] == 'l' && ret1[2] == 'i')
            {
                flag_i = true;
                flag_l = true;
                printf("flag i and l is true\n");
            }
        }
        else
        {
            printf("Not the correct arguments, try 'i' or 'l'\n");
        }
    }
    else if(argc == 4)
    {
        arg1_len = strlen(argv[2]);
        arg2_len = strlen(argv[3]);
        
        str1 = argv[2];
        ret1 = strchr(str1, ch);

        str2 = argv[3];
        ret2 = strchr(str2, ch);

        printf("String after |%c| is: |%s|\n", ch, ret1);
        printf("String after |%c| is: |%s|\n", ch, ret2);

        if (arg1_len == 2 && arg2_len == 2)
        {
            if (ret1[1] == 'i' && ret2[1] == 'l')
            {
                flag_i = true;
                flag_l = true;
                printf("flag i and l is true\n");
            }
            else if (ret1[1] == 'l' && ret2[1] == 'i')
            {
                flag_i = true;
                flag_l = true;
                printf("flag i and l is true\n");
            }
            else
            {
                printf("Not the correct arguments, try 'i' or 'l' line 82\n");
            }
        }
        else
        {
            printf("Not the correct arguments, try 'i' or 'l' line 87\n");
        }
    }
    else
    {
        printf("Not the correct arguments, try 'i' or 'l' line 105\n");
    }

    if (!flag_i && !flag_l)
        ls(path);
    else if (flag_i && !flag_l)
        lsi(path);
    else if (!flag_i && flag_l)
        lsl(path);
    else if (flag_i && flag_l)
        lsil(path);

    return 0;
    }