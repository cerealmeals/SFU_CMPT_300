#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void ls(char* path){

    char* cwd = path;
    if(cwd[0] == '\0'){
        if (getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd() error");
            return;
        }
    }

    DIR* directory_stream = opendir(cwd);
    if(directory_stream == NULL){
        printf("Error: %s is not a valid path\n", cwd);
        return;
    }

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

void lsi(char* path){

    char* cwd = path;
    if(cwd[0] == '\0'){
        if (getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd() error");
            return;
        }
    }

    DIR* directory_stream = opendir(cwd);
    if(directory_stream == NULL){
        printf("Error: %s is not a valid path\n", cwd);
        return;
    }
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

void lsl(char* path){

    char* cwd = path;
    if(cwd[0] == '\0'){
        if (getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd() error");
            return;
        }
    }

    DIR* directory_stream = opendir(cwd);
    if(directory_stream == NULL){
        printf("Error: %s is not a valid path\n", cwd);
        return;
    }
    struct dirent* readable = readdir(directory_stream);
    struct stat info;
    
    char slash[2] = "/\0";
    strcat(cwd, slash);
    for(;readable != NULL; readable = readdir(directory_stream)){

        char path_of_file[PATH_MAX];
        
        strcpy(path_of_file, cwd);
        strcat(path_of_file, readable->d_name);
        
        int result = lstat(readable->d_name, &info);
        
        if(readable->d_name[0] != '.'){
            if(result == 0){
                
                output_l(&info);
                
                printf(" %s", readable->d_name);

                if(S_ISLNK(info.st_mode)){
                    char buf[PATH_MAX];
                    int check = readlink(readable->d_name, buf, PATH_MAX);
                    if(check != -1){
                        buf[check] = '\0';
                    }
                    else{
                        printf("Error readlink-lsl\n");
                    }
                    printf(" -> %s\n", buf);
                }
                else{
                    printf("\n");
                }
                
            }
            else{
                printf("Error with getting stats from %s, the error is: %s\n", readable->d_name, strerror(errno));
            }
        }
    }

    closedir(directory_stream);
    return;

}

void lsil(char* path){

    char* cwd = path;
    if(cwd[0] == '\0'){
        if (getcwd(cwd, PATH_MAX) == NULL) {
            perror("getcwd() error");
            return;
        }
    }

    DIR* directory_stream = opendir(cwd);
    if(directory_stream == NULL){
        printf("Error: %s is not a valid path\n", cwd);
        return;
    }
    struct dirent* readable = readdir(directory_stream);
    struct stat info;
    
    char slash[2] = "/\0";
    strcat(cwd, slash);
    for(;readable != NULL; readable = readdir(directory_stream)){

        char path_of_file[PATH_MAX];
        
        strcpy(path_of_file, cwd);
        strcat(path_of_file, readable->d_name);
        
        int result = lstat(readable->d_name, &info);
        
        if(readable->d_name[0] != '.'){
            if(result == 0){
                
                printf("%ld ", readable->d_ino);

                output_l(&info);
                
                printf(" %s", readable->d_name);

                if(S_ISLNK(info.st_mode)){
                    char buf[PATH_MAX];
                    int check = readlink(readable->d_name, buf, PATH_MAX);
                    if(check != -1){
                        buf[check] = '\0';
                    }
                    else{
                        printf("Error readlink-lsl\n");
                    }
                    printf(" -> %s\n", buf);
                }
                else{
                    printf("\n");
                }
                
            }
            else{
                printf("Error with getting stats from %s, the error is: %s\n", readable->d_name, strerror(errno));
            }
        }
    }

    closedir(directory_stream);
    return;
}

void output_l(struct stat* info){

// column one
    mode_t mode = info->st_mode;
    // directory?
    if(S_ISDIR(info->st_mode)){
        printf("d");
    }
    // soft link?
    else if(S_ISLNK(info->st_mode)){
        printf("l");
    }
    // file?
    else{
        printf("-");
    }
    // user read?
    if_else_permisions(mode, S_IRUSR, 'r');
    
    // user write?
    if_else_permisions(mode, S_IWUSR, 'w');

    // user excute?
    if_else_permisions(mode, S_IXUSR, 'x');
    
    // group read?
    if_else_permisions(mode, S_IRGRP, 'r');
    
    // group write?
    if_else_permisions(mode, S_IWGRP, 'w');

    // group excute?
    if_else_permisions(mode, S_IXGRP, 'x');

    // others read?
    if_else_permisions(mode, S_IROTH, 'r');
    
    // others write?
    if_else_permisions(mode, S_IWOTH, 'w');

    // others excute?
    if_else_permisions(mode, S_IXOTH, 'x');

// column 2
    printf(" %ld", info->st_nlink);

// column 3
    struct passwd *pw = NULL;
    pw = getpwuid(info->st_uid);
    printf(" %s", pw->pw_name);

// column 4
    struct group *grp;
    grp = getgrgid(info->st_gid); 
    printf(" %s", grp->gr_name);

// column 5
    printf(" %5ld", info->st_size);

// column 6
    struct tm* time = localtime(&(info->st_mtim.tv_sec));
    char month[10];
    char year_and_time[15];
    strftime(month, 10, "%b", time);
    strftime(year_and_time, 15, "%Y %H:%M", time);
    printf(" %s %2d %s", month, time->tm_mday, year_and_time);

}

void if_else_permisions(mode_t mode, int mask, char ch){
    if((mode & mask)){
        printf("%c", ch);
    }
    else{
        printf("-");
    }
}