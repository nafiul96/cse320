#include "kyle.h"
#include "allocate.h"
#include "imprimer.h"
#include"stdlib.h"
#include"stdio.h"
char * memerr = "memerr";



imp_node *newtype(){

    imp_node *p;

    if((p = (imp_node *)malloc(sizeof(imp_node))) == NULL){

        char *buff = malloc(1024);
        buff = imp_format_error_message(memerr, buff, 1024);
        fprintf(stderr, "%s\n",buff);
        free(buff);

    }
    return p;
}



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