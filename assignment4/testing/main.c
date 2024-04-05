#include <stdio.h>

int main(int argc, char *argv[]){

    bool flag_i = false;
    bool flag_l = false;
    int arg1_len = 0;
    int arg2_len = 0;

    if(argc >= 1){
        arg1_len = strlen(argv[1])
        if(argv[1][1] == 'i' && argv[1][1] == 'l'){
            if(argv[1][1] == 'i'){
                flag_i = true;
            }
            if(argv[1][1] == 'l'){
                flag_l = true;
            }
            
        }
        else{
            printf("Not the correct arguments, try 'i' or 'l'\n")
        }
    }
    if(argc >= 2){
        arg2_len = strlen(argv[2])
        if(argv[2][1] == 'i' && argv[2][1] == 'l'){
            if(argv[1][1] == 'i'){
                flag_i = true;
            }
            if(argv[1][1] == 'l'){
                flag_l = true;
            }
        }
        else{
            printf("Not the correct arguments, try 'i' or 'l'\n")
        }
    }

    

    return 0;
}