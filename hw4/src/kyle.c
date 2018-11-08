#include <stdlib.h>
#include <stdio.h>
#include "kyle.h"
#include "string.h"
#include "malloc.h"






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




void ctot(char bfr[], char **collector, int * tokensz){

    int i = 0;
    char *p = strtok (bfr, " ");

    // need to limit the number of elemnent the buffer can handle

    while(p != NULL){

        collector[i++] = p;
        p = strtok(NULL, " ");
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

