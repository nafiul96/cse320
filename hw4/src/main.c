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
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include "graph.h"
#include "<time.h>"




/*
 * "Imprimer" printer spooler.
 */



int inp =0, out = 0, quit = 0, tp, printer, job;


PRINTER  *printers[32];
PRINTER_SET printer_set;

int main(int argc, char *argv[])
{
    printer = 0;
    tp = 0;
    printer_set = 0;
    job = 0;

    g = creategraph();

    createQ();


   // char *inputfile;
    char optval;
    while(optind < argc) {
	if((optval = getopt(argc, argv, "i:o:")) != -1) {
	    switch(optval) {
        case 'i':
        //get the file name
        //inputfile = optarg;
        ifile = fopen(optarg,"r");
        if(ifile == NULL){
            fprintf(stderr, "error\n");
        }else{
            inp++;
        }

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


    int len =0;

    while(!quit){


            char *buffer = readline("imp>");
            char **collector = malloc(4048);
            ctot(buffer, collector,&len, " ");


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

                // if(typeexists(env_type, collector[1], tp) > 0 ){
                if(findbytype(collector[1]) != NULL ){
                    // show error message

                }else{


                    gnode *temp = malloc(sizeof(gnode));
                    populatenode(temp, collector[1]);
                    addgnode(g, temp);

                }

                }else{

                }

        }else if(strcmp(collector[0],"printer")== 0){



            if(len == 3){


                if(findbytype(collector[2]) != NULL){


                    PRINTER *ptr = newprinter();
                    ptr->id = printer;

                    char *b = newstring(strlen(collector[1]));
                    char *c = newstring(strlen(collector[2]));
                    strcpy(b, collector[1]);
                    strcpy(c, collector[2]);

                    ptr->name  = b;
                    ptr->type  = c;
                    ptr->enabled = 1;
                    ptr->busy = 0;
                    printer_set  |= (1<<printer);
                    printers[printer++] = ptr;
                }

            }


        }else if(strcmp(collector[0],"conversion")== 0){

            // assumed to be working
            if(len >=4){

                conv *conversion = malloc( sizeof(conv));
                char *p = newstring( strlen(collector[3])   );

                if( linkme(collector[1], collector[2], conversion) == 0){
                    //show error message
                }


            }




        }else if(strcmp(collector[0],"printers") == 0){

            if(len == 1){
                all_printers(printers, printer);
            }



        }else if(strcmp(collector[0],"jobs") == 0){

            if(len== 1){

                printjobs();

            }



        }else if(strcmp(collector[0],"print") == 0){
            if(len > 1){
                printf("recognized:%s \n",buffer);

                //check conditions
            //     pid_t rootProc = fork();
            //     if (converter(NULL) != 0){
            //         printf("abort process%d\n", rootProc);
            //     }

            // }

                //testing the queue

//                 typedef struct job {
//     int jobid;                      /* Job ID of job. */
//     JOB_STATUS status;              /* Status of job. */
//     int pgid;                       /* Process group ID of conversion pipeline. */
//     char *file_name;                /* Pathname of file to be printed. */
//     char *file_type;                /* Type name of file to be printed. */
//     PRINTER_SET eligible_printers;  /* Set of eligible printers for the job. */
//     PRINTER *chosen_printer;        /* Printer chosen for this job. */
//     struct timeval creation_time;   /* Time job was queued. */
//     struct timeval change_time;     /* Time of last status change. */
//     void *other_info;               /* You may store other info in this field. */
// } JOB;




                JOB *j = newjob();
                j->jobid = job;
                j->status = QUEUED;
                // j->pgid = job;
                char *fname = newstring(strlen(collector[1]));
                strcpy(fname, collector[1]);
                j->file_type = fname;
                j->eligible_printers = ANY_PRINTER;
                j->chosen_printer = printers[0];

                char *today = NULL;
                time_t now;
                time(&now);
                today = ctime(&now);



                jobnode *node = newjobnode();
                node->j = j;
                insertjob(node);


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

