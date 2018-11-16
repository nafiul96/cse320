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
#include "jobq.h"

//#include "<time.h>"




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

    jobq *q = createQ();


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
                    char *tmp = newstring(255);
                    imp_format_error_message("type exists!", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);

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

            // assumed to be working - still not taking acocunt the program part
            if(len >=4){


                gnode *f = findbytype(collector[1]);
                gnode *t = findbytype(collector[2]);

                if( f == NULL){
                    char *tmp = newstring(255);
                    imp_format_error_message("type does not exist!\n", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);

                    continue;

                }

                if( t == NULL){
                    char *tmp = newstring(255);
                    imp_format_error_message("type does not exist!\n", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);
                    continue;

                }

                if( findadjbytype(f->list->head,t->n->type) != NULL){

                    char *tmp = newstring(255);
                    imp_format_error_message("edge exists \n", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);
                    continue;
                }


                    conv *conversion = malloc( sizeof(conv));
                    char *prg = malloc(sizeof(255));
                    strcpy(prg, collector[3]);
                    conversion->args = NULL;


                if( linkme(collector[1], collector[2], conversion) == 0){
                    //show error message
                    char *tmp = newstring(255);
                    imp_format_error_message("conversion not possible!", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);
                    continue;

                }


            }




        }else if(strcmp(collector[0],"printers") == 0){

            if(len == 1){
                all_printers(printers, printer);
            }



        }else if(strcmp(collector[0],"jobs") == 0){

            if(len== 1){

                printQ(q);

            }



        }else if(strcmp(collector[0],"print") == 0){





            if(len > 1){
                printf("recognized:%s \n",buffer);


                char *pf = newstring(strlen(collector[1]));
                strcpy(pf,collector[1]);
                char **f_array = malloc(1024);
                int fl = 0;
                ctot(pf, f_array,&fl, ".");
                char *ext = newstring( strlen(f_array[1]) );
                strcpy(ext,f_array[1]);
                if(fl<2){
                    char *tmp = newstring(255);
                    imp_format_error_message("not a valid filename\n", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);


                }
                else if( findbytype(ext) == NULL){
                    char *tmp = newstring(255);
                    imp_format_error_message("cannot print this type!\n", tmp, 255);
                    printf("%s\n",tmp);
                    free(tmp);

                }else{

                JOB *j = newjob();
                j->jobid = job;
                j->status = QUEUED;
                 j->pgid = job;
                char *fname = newstring(strlen(collector[1]));
                strcpy(fname, collector[1]);
                j->file_type = fname;
                j->eligible_printers = ANY_PRINTER;
                j->chosen_printer = printers[0];
                jobnode *node = malloc( sizeof(jobnode));
                node->j = j;


                if(len >2){

                    //typedef uint32_t PRINTER_SET;
                    PRINTER_SET *myset = malloc( sizeof(uint32_t));
                    *myset = 0x0;

                    for(int i= 2; i< len; i++){

                        for(int j=0; j<printer; j++){

                            if(  strcmp(printers[j]->name,collector[i]) ==0   && findpath(g,ext,printers[j]->type) != NULL ){
                                //printer_set  |= (1<<printer);
                                *myset |= (1<<printers[j]->id);
                            }

                        }
                    }

                    j->eligible_printers = *myset;

                }
                enqueue(q,node);
                printf("job has been qeuued successfully\n");


            }


            }





        }else if(strcmp(collector[0],"cancel") == 0){
            if(len ==2){
                printf("recognized:%s \n",buffer);
                //find the job by
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

        if(q->count >0){
        jobnode *jpt;
        for(jpt = q->front; jpt != NULL; jpt = jpt->next){

            // check for completeted to be removed!
            if(jpt->j->status == COMPLETED){

                //check the time status
                //
                continue;

            }
            //queued
            if(jpt->j->status == QUEUED){

                for(int i=0; i<printer; i++){

                    if( printers[i]->enabled &&
                        //!printers[i]->busy  &&
                        (printers[i]->id & jpt->j->eligible_printers) > 0 &&
                        findpath(g,   jpt->j->file_type,   printers[i]->type) != NULL){

                        jpt->j->chosen_printer = printers[i];

                       break;
                    }
                }

            }
        }


        if(jpt != NULL){


            pid_t root = fork();
            if( root < 0  ){
                abort();
            }

            if(root == 0 ){
                pathnode *p = findpath(g, jpt->j->file_type, jpt->j->chosen_printer->type);
                converter( p,  jpt->j);
            }
        }

    }


    }



    exit(EXIT_SUCCESS);
}



