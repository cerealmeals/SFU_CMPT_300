#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "functions.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[])
{
    bool flag_i = false;
    bool flag_l = false;
    // int arg1_len = 0;
    // int arg2_len = 0;

    char path[PATH_MAX] = "\0";
    //printf("path_max %d", PATH_MAX);
    // char ch = '-';                                                                                                                                              
    // char *ret1;
    // char *ret2;

    printf("argc #: %d\n", argc);


    if (argc < 2)
    {
        ls(path);
    }
    // else if (argc == 2)
    // {
    //     if (strcmp(argv[1], "ls") == 0){
    //         ls(path);
    //     }
    // }
    else if(argc == 2)
    {                                                                                                                                          
        //arg1_len = strlen(argv[2]);
        //ret1 = strchr(argv[2], ch);
        //printf("String after |%c| is: |%s|\n", ch, ret1);
        //printf("length of arg1: %d\n", arg1_len);

        if (strcmp(argv[1], "-i") == 0)
        {
            flag_i = true;
            printf("flag i is true\n");
        }
        else if (strcmp(argv[1], "-l") == 0)
        {
            flag_l = true;
            printf("flag l is true\n");
        }
        else if (strcmp(argv[1], "-il") == 0 || strcmp(argv[1], "-li") == 0)
        {
            flag_i = true;
            flag_l = true;
            printf("flag i and l is true\n");
        }
        else
        {
            printf("Not the correct arguments, try 'i' or 'l'\n");
        }
    }
    else if(argc == 3)
    {
        // arg1_len = strlen(argv[2]);
        // arg2_len = strlen(argv[3]);
        
        // ret1 = strchr(argv[2], ch);
        // ret2 = strchr(argv[3], ch);

        // printf("String after |%c| is: |%s|\n", ch, ret1);
        // printf("String after |%c| is: |%s|\n", ch, ret2);

        if ((strcmp(argv[1], "-i")==0 && strcmp(argv[2], "-l")==0) || (strcmp(argv[1], "-l")==0 && strcmp(argv[2], "-i")==0))
        {
            flag_i = true;
            flag_l = true;
            printf("flag i and l is true\n");
        }
        else if (strcmp(argv[1], "-i")==0 && strcmp(argv[2], "-l")!=0)
        {
            // case: ./UnixLS ls -i path 
            flag_i = true;
            strcpy(path, argv[2]);
            printf("flag i is true, getting path: %s\n", path);
        }
        else if (strcmp(argv[1], "-l")==0 && strcmp(argv[2], "-i")!=0)
        {
            // case: ./UnixLS ls -l path
            flag_l = true;
            strcpy(path, argv[2]);
            printf("flag l is true, getting path: %s\n", path);
        } 
        else if (strcmp(argv[1], "-il")==0 || strcmp(argv[1], "-li")==0)
        {
            // case: ./UnixLS ls -il path or ./UnixLS ls -li path
            flag_i = true;
            flag_l = true;
            strcpy(path, argv[2]);
            printf("flag i and l is true, getting path: %s\n", path);
        }
        else
        {
            printf("Not the correct arguments, try 'i' or 'l' line 87\n");
        }
    }
    else if(argc == 4)
    {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[2], "-l") == 0)
        {
            flag_i = true;
            flag_l = true;
            strcpy(path, argv[3]);
            printf("flag i and l is true, getting path: %s\n", path);
        }
        else if (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "-i") == 0)
        {
            flag_i = true;
            flag_l = true;
            strcpy(path, argv[3]);
            printf("flag i and l is true, getting path: %s\n", path);
        }
    }
    else
    {
        printf("Not the correct arguments, try 'i' or 'l' line 105\n");
    }

    if (flag_i && !flag_l)
    {
        printf("lsi()\n");
        lsi(path);
    }
    else if (!flag_i && flag_l)
        lsl(path);
    else if (flag_i && flag_l)
        lsil(path);

    return 0;
}