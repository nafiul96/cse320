#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include"imprimer.h"
#include"string.h"
#include"readline/readline.h"
#include"malloc.h"



/*
 * "Imprimer" printer spooler.
 */
void printCommand();
void ctot(char bfr[], char **collector, int * tokensz);


int inp =0, out = 0, quit = 0, tp = 0, printer = 0;

char *env_type[1024];
PRINTER  *printers[32];


int main(int argc, char *argv[])
{

    //char *inputfile;
    char optval;
    while(optind < argc) {
	if((optval = getopt(argc, argv, "i:o:")) != -1) {
	    switch(optval) {
        case 'i':
        //get the file name
       // inputfile = optarg;
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

    //FILE *ifile;

    if(inp){
        // ifile = fopen(inputfile,"r");
        // stdin = ifile;
    }
    char*buffer;
    char *collector[1024];
    int len =0;

    while(!quit){

        if(inp){

            scanf("%[^\n]%*c", buffer);
            if(*buffer == EOF){
                break;
            }

        }
       else{
            buffer = readline("imp>");
            ctot(buffer, collector,&len);


       }
        if(strcmp(collector[0],"help") == 0){
            printCommand();

        }else if(strcmp(collector[0],"quit") == 0){
            quit++;

        }else if(strcmp(collector[0],"type") == 0){
            env_type[tp] = collector[1];
            tp++;



        }else if(strcmp(collector[0],"printer")== 0){
            printers[printer]->id = printer;
            printers[printer]->name  = collector[1];
            printers[printer]->type  = collector[2];
        }else if(strcmp(collector[0],"conversion")== 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"printers") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"jobs") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"print") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"cancel") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"pause") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"resume") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"disable") == 0){
            printf("recognized:%s \n",buffer);

        }else if(strcmp(collector[0],"enable")== 0){
            printf("recognized:%s \n",buffer);

        }

    }




    exit(EXIT_SUCCESS);
}



void printCommand(){

    printf("Miscellaneous Commands\n");
    printf("help \n quit\n");
    printf("Configuration Commands \n");
    printf("type file_name \n printer printer_name file_type \n");
    printf("conversion file_type1 file_type2 coversion_program[arg1 arg2 ...] \n");
    printf("Informational Commands \n printers \n jobs \n");
    printf("Spooling Commands\n");
    printf("print file_name [printer1 printer2 ...]\n");
    printf("cancel job_number \n");
    printf("pause job_number \n");
    printf("resume job_number \n");
    printf("disable printer_name \n");
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

