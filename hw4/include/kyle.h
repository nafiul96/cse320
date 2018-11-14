#ifndef KYLE_H
#define KYLE_H

#include "imprimer.h"
#include <stdint.h>
#include <stdlib.h>

// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>


FILE *ifile;

// typedef struct {

//     int matrix_id;
//     char * type;

// }__attribute__((packed)) imp_node;



// typedef struct {
// char *name;
// char **args;
// }__attribute__((packed)) conv;








//int typeexists(imp_node *data[], char * inptype, int count );
void printCommand();
void ctot(char bfr[], char **collector, int * tokensz, char *del);
void all_printers(PRINTER *ptr[], int num);
int converter(int pathlength);




#endif
