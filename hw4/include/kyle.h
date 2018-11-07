#ifndef KYLE_H
#define KYLE_H

#include "imprimer.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {

    char * type;
    struct imp_node *head;


}__attribute__((packed)) imp_node;



int typeexists(imp_node *data[], char * inptype, int count );




void printCommand();
void ctot(char bfr[], char **collector, int * tokensz);
void all_printers(PRINTER *ptr[], int num);


#endif
