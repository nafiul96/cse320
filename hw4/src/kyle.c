#include <stdlib.h>
#include <stdio.h>
#include "kyle.h"
#include "string.h"
#include "malloc.h"
#include "imprimer.h"

// C program to demonstrate use of fork() and pipe()
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>






void printCommand(){

    printf("Miscellaneous Commands\n");
    printf("help\nquit\n");
    printf("Configuration Commands\n");
    printf("type file_name\n printer printer_name file_type\n");
    printf("conversion file_type1 file_type2 coversion_program[arg1 arg2 ...]\n");
    printf("Informational Commands\nprinters\njobs\n");
    printf("Spooling Commands\n");
    printf("print file_name [printer1 printer2 ...]\n");
    printf("cancel job_number\n");
    printf("pause job_number\n");
    printf("resume job_number\n");
    printf("disable printer_name\n");
    printf("enable printer_name\n");


}




void ctot(char bfr[], char **collector, int * tokensz, char *del){

    int i = 0;
    char *p = strtok (bfr, del);

    // need to limit the number of elemnent the buffer can handle

    while(p != NULL){

        collector[i++] = p;
        p = strtok(NULL, del);
    }

    *tokensz = i;
}


void all_printers(PRINTER *ptr[], int num){

    for(int i=0; i<num; i++){
        PRINTER *samsung = ptr[i];
        char *buff = malloc(4048);
        buff = imp_format_printer_status(samsung, buff, 4048);
        printf("%s\n", buff);
        free(buff);

    }


}




















/*
takes the array of recognizable types and its size, and user provided  type
@returns the matrix_id if exists, else returns -1
*/

int typeexists(imp_node *data[], char *inptype, int count ){

    if(count <= 0)
        return -1;

    for(int i=0; i< count; i++){

        imp_node *tmp = data[i];

        if( strcmp(tmp->type, inptype) == 0 ){
            return tmp->matrix_id;
        }
    }

    return -1;
}


int converter(int pathlength){


    // create pipes
    int pipes[2];
    int pipes2[2];

    //set group id
    // if(setgpid(0,0) == -1)
    //     exit(1);


    pid_t id;
    //setup the pipe
    if(pipe(pipes) == -1 || pipe(pipes2) ==-1)
        exit(1);

    for (int i=0; i<=pathlength; i++){

        if( (id = fork()) <0 )
            exit(1);//fuck off

        if( id == 0  ){    // read from parent stdin

            // welcome aboard - read from pipe0(in-3) write to pipe2(out-6)
            dup2(pipes[0], 0);
            dup2(pipes2[1], 1);

            //close them all since we only need
            close(pipes[0]);
            close(pipes[1]);
            close(pipes2[0]);
            close(pipes2[1]);
            //execute the program


            //swap two pipes pipe1[5,6] <====> pipe2[3,4]
            int r2 = pipes2[0];
            int w2 = pipes2[1];
            pipes2[0] = pipes[0];
            pipes2[1] = pipes[1];
            pipes[0] = r2;
            pipes[1] = w2;

            //exit success



        }


    }


    close(pipes[0]);
    close(pipes[1]);
    close(pipes2[0]);
    close(pipes2[1]);
    dup2(pipes[0], 0);
    //dup2(pipes2[1], connect_to_printer(NULL,0x0));


    //for(i=0; i <)





    return 1;

}




























