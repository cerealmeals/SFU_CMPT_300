#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void ls(){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    DIR* directory_stream = opendir(cwd);

    struct dirent* readable = readdir(directory_stream);
    
    for(;readable != NULL; readable = readdir(directory_stream)){
        
        if(readable->d_name[0] != '.'){
            printf("%s  ", readable->d_name);
        }
        
    }
    printf("\n");
    closedir(directory_stream);
    return;
}

void lsi(){

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    DIR* directory_stream = opendir(cwd);
    struct dirent* readable = readdir(directory_stream);

    
    for(;readable != NULL; readable = readdir(directory_stream)){
        
        if(readable->d_name[0] != '.'){
            printf("%ld %s  ", readable->d_ino, readable->d_name);
        }
    }

    printf("\n");
    closedir(directory_stream);
    return;

}

void lsl(){

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return;
    }

    DIR* directory_stream = opendir(cwd);
    struct dirent* readable = readdir(directory_stream);
    struct stat info;
    
    for(;readable != NULL; readable = readdir(directory_stream)){
        
        if(readable->d_name[0] != '.'){
            printf("%ld %s  ", readable->d_ino, readable->d_name);
        }
    }

    printf("\n");
    closedir(directory_stream);
    return;

}

void lsil(){


}