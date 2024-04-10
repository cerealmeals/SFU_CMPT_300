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

    bool path_flag = false;
    char path[PATH_MAX] = "\0";

    if (argc < 2)
    {
        ls(path);
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {

            // take the first argument

            // parse element
            // check to see if it has '-' in front
            // check letters
            // if i or l set bool's to true
            // if anything else return error

            // if not starting with '-' it is a path
            // give path to function
            // set path_flag to flase
            if (path_flag = false)
            {
                if (strcmp(argv[i], "-i") == 0)
                {
                    flag_i = true;
                }
                else if (strcmp(argv[i], "-l") == 0)
                {
                    flag_l = true;
                }
                else if (strcmp(argv[i], "-il") == 0)
                {
                    flag_i = true;
                    flag_l = true;
                }
                else if (strcmp(argv[i], "-li") == 0)
                {
                    flag_i = true;
                    flag_l = true;
                }
                else
                {
                    path_flag = true;
                    strcpy(path, argv[i]);
                    ls(path);
                }
            }
            else
            {
                strcpy(path, argv[i]);
                if (flag_i)
                {
                    lsi(path);
                }
                else if (flag_l)
                {
                    lsl(path);
                }
                else if (flag_i && flag_l)
                {
                    lsil(path);
                }
                else
                {
                    ls(path);
                }
            }
        }

        if (path_flag)
        {
            if (flag_i)
            {
                lsi();
            }
            else if (flag_l)
            {
                lsl();
            }
            else if (flag_i && flag_l)
            {
                lsil();
            }
        }
    }
}