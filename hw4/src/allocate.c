#include "kyle.h"
#include "allocate.h"
#include "imprimer.h"
#include"stdlib.h"
#include"stdio.h"



char * memerr = "memerr";



// imp_node *newtype(){

//     imp_node *p;

//     if((p = (imp_node *)malloc(sizeof(imp_node))) == NULL){

//         char *buff = malloc(1024);
//         buff = imp_format_error_message(memerr, buff, 1024);
//         fprintf(stderr, "%s\n",buff);
//         free(buff);

//     }
//     return p;
// }



PRINTER *newprinter(){

    PRINTER *p;

    if((p = (PRINTER *)malloc(sizeof(PRINTER))) == NULL){

        char *buff = malloc(1024);
        buff = imp_format_error_message(memerr, buff, 1024);
        fprintf(stderr, "%s\n",buff);
        free(buff);

    }
    return p;


}

void *newstring(int len){

    void *p;
    if((p = (void *)malloc(len)) == NULL){

        char *buff = malloc(1024);
        buff = imp_format_error_message(memerr, buff, 1024);
        fprintf(stderr, "%s\n",buff);
        free(buff);

    }
    return p;

}

// conv *newprogram(){

//     conv *p;
//     if((p = (conv *)malloc(sizeof(conv))) == NULL){

//         char *buff = malloc(1024);
//         buff = imp_format_error_message(memerr, buff, 1024);
//         fprintf(stderr, "%s\n",buff);
//         free(buff);

//     }
//     return p;

// }


JOB * newjob(){

    JOB *p;
    if((p = (JOB *)malloc(sizeof(JOB))) == NULL){

        char *buff = malloc(1024);
        buff = imp_format_error_message(memerr, buff, 1024);
        fprintf(stderr, "%s\n",buff);
        free(buff);

    }
    return p;



}


// jobnode *newjobnode(){

//     jobnode *p;
//     if((p = (jobnode *)malloc(sizeof(jobnode))) == NULL){

//         char *buff = malloc(1024);
//         buff = imp_format_error_message(memerr, buff, 1024);
//         fprintf(stderr, "%s\n",buff);
//         free(buff);

//     }
//     return p;



// }