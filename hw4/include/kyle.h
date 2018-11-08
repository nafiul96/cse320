#ifndef KYLE_H
#define KYLE_H

#include "imprimer.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {

    int matrix_id;
    char * type;

}__attribute__((packed)) imp_node;



typedef struct {
char *name;
char **args;
}__attribute__((packed)) conv;








int typeexists(imp_node *data[], char * inptype, int count );
void printCommand();
void ctot(char bfr[], char **collector, int * tokensz);
void all_printers(PRINTER *ptr[], int num);




#endif
