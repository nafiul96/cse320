#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include"imprimer.h"
#include"string.h"
#include"readline/readline.h"
#include"malloc.h"
#include "kyle.h"
#include "imprimer.h"
#include "allocate.h"



/*
 * "Imprimer" printer spooler.
 */



int inp =0, out = 0, quit = 0, tp, printer;

//char *env_type[1024];
imp_node **env_type;
char ***matrix;

PRINTER  *printers[32 * sizeof(PRINTER)];


int main(int argc, char *argv[])
{
    printer = 0;
    tp = 0;

    env_type = malloc(50 * sizeof(imp_node));
    matrix = malloc(50 * 50 * sizeof(int));

   // char *inputfile;
    char optval;
    while(optind < argc) {
	if((optval = getopt(argc, argv, "i:o:")) != -1) {
	    switch(optval) {
        case 'i':
        //get the file name
        //inputfile = optarg;
        inp++;
        break;
        case 'o':
        // get the file name
        out++;
        break;
	    case '?':
		fprintf(stderr, "Usage: %s [-i <cmd_file>] [-o <out_file>]\n", argv[0]);
		exit(EXIT_FAILURE);
		break;
	    default:
		break;
	    }
	}

    }

    // FILE *ifile;
    // if(inp){
    //     ifile  = fopen(inputfile, "r");
    // }
    //char *buffer;
    //char **collector;

    int len =0;

    while(!quit){



            char *buffer = readline("imp>");
            char **collector = malloc(4048);
            ctot(buffer, collector,&len);

       //}



        if(strcmp(collector[0],"help") == 0){

            if(len == 1){
                printCommand();
            }


        }else if(strcmp(collector[0],"quit") == 0){

            if(len == 1){
                quit++;
            }



        }else if(strcmp(collector[0],"type") == 0){

            if(len == 2){

                if(typeexists(env_type, collector[1], tp) < 0 ){

                }else{


                    imp_node *typeptr = newtype();
                    char *tpy = malloc(1);
                    strcpy(tpy, collector[1]);
                    typeptr->type = tpy;
                    typeptr->matrix_id = tp;
                    matrix[tp] = 0;
                    env_type[tp++] = typeptr;
                    }
                }else{
                    //internal error occured!
                }

        }else if(strcmp(collector[0],"printer")== 0){


            // verify the length
            if(len == 3){

                PRINTER *ptr = newprinter();
                ptr->id = printer;

                char *b = malloc(1);
                char *c = malloc(1);
                strcpy(b, collector[1]);
                strcpy(c, collector[2]);

                ptr->name  = b;
                ptr->type  = c;
                ptr->enabled = 1;
                ptr->busy = 0;
                printers[printer++] = ptr;
            }


        }else if(strcmp(collector[0],"conversion")== 0){

            // assumed to be working, needs to add conversion program!!!
            if(len >=4){

                int indx = typeexists(env_type, collector[1], tp);
                int indy = typeexists(env_type, collector[2], tp);
                if(indx>=0 && indy >= 0){
                    char *edge = malloc(1);
                    edge = "yes";
                    matrix[indx][indy] = edge;
                }

            }




        }else if(strcmp(collector[0],"printers") == 0){

            if(len == 1){
                all_printers(printers, printer);
            }



        }else if(strcmp(collector[0],"jobs") == 0){

            if(len== 1){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"print") == 0){
            if(len > 1){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"cancel") == 0){
            if(len ==2){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"pause") == 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"resume") == 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"disable") == 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }



        }else if(strcmp(collector[0],"enable")== 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }



        }

        free(buffer);
        free(collector);

    }




    exit(EXIT_SUCCESS);
}



// void printCommand(){

//     printf("Miscellaneous Commands\n");
//     printf("help \n quit\n");
//     printf("Configuration Commands \n");
//     printf("type file_name \n printer printer_name file_type \n");
//     printf("conversion file_type1 file_type2 coversion_program[arg1 arg2 ...] \n");
//     printf("Informational Commands \n printers \n jobs \n");
//     printf("Spooling Commands\n");
//     printf("print file_name [printer1 printer2 ...]\n");
//     printf("cancel job_number \n");
//     printf("pause job_number \n");
//     printf("resume job_number \n");
//     printf("disable printer_name \n");
//     printf("enable printer_name\n");


// }




// void ctot(char bfr[], char **collector, int * tokensz){

//     int i = 0;
//     char *p = strtok (bfr, " ");

//     // need to limit the number of elemnent the buffer can handle

//     while(p != NULL){

//         collector[i++] = p;
//         p = strtok(NULL, " ");
//     }

//     *tokensz = i;
// }


// void all_printers(PRINTER *ptr[], int num){

//     for(int i=0; i<num; i++){
//         PRINTER *samsung = ptr[i];
//         char *buff = malloc(1024);
//         buff = imp_format_printer_status(samsung, buff, 1024);
//         printf("%s\n", buff);
//         free(buff);

//     }


// }

