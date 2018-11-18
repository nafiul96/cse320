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
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>



/*
 * "Imprimer" printer spooler.
 */

void forkmaster(pathnode *path, jobnode *j);
void handler(int sigint);
jobnode *findjobbyid(int id);
void updatejobstat(jobnode *j, JOB_STATUS status);


int inp =0, out = 0, quit = 0, tp, printer, job;

jobq *q;

PRINTER  *printers[32];
PRINTER_SET printer_set;

int main(int argc, char *argv[])
{
    printer = 0;
    tp = 0;
    printer_set = 0;
    job = 0;

    g = creategraph();

    q = createQ();


    char *inputfile;
    char optval;
    while(optind < argc) {
	if((optval = getopt(argc, argv, "i:o:")) != -1) {
	    switch(optval) {
        case 'i':
        //get the file name
        inputfile = optarg;
        ifile = fopen(inputfile,"r");
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




            char *buffer;
            char **collector;

            if(inp){

                buffer = malloc(1024);

                char buff[1024];
                //buffer  = buff;
                if( ( fgets(buff, 1024,ifile)) ==NULL){
                    fclose(ifile);
                    inp = 0;
                }else{


                char *token = malloc(4048);

                token  = strtok(buff, "\n");

                strcpy(buffer,token);
                collector = malloc(4048);
                ctot(buffer, collector,&len, " ");
                }

            }else{

                buffer = readline("imp>");
                collector = malloc(4048);
                ctot(buffer, collector,&len, " ");
            }


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
                    conversion->args = &collector[3];

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

            free(buffer);
            free(collector);


        }else if(strcmp(collector[0],"jobs") == 0){

            if(len== 1){

                printQ(q);

            }
            free(buffer);
            free(collector);



        }else if(strcmp(collector[0],"print") == 0){





            if(len > 1){
               // printf("recognized:%s \n",buffer);


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
                j->file_name = fname;
                j->file_type = ext;
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

                            if(  strcmp(printers[j]->name,collector[i]) ==0){
                                //printer_set  |= (1<<printer);
                                *myset |= (1<<printers[j]->id);
                            }

                        }
                    }

                    j->eligible_printers = *myset;

                }
                gettimeofday( &(j->creation_time), NULL);
                enqueue(q,node);
                char *jstat = malloc(1024);
                imp_format_job_status(q->front->j, jstat, 1024);
                printf("%s\n",jstat);


            }


            }


            free(buffer);
            free(collector);



        }else if(strcmp(collector[0],"cancel") == 0){
            if(len ==2){
                printf("recognized:%s \n",buffer);
                //find the job by
            }

            free(buffer);
            free(collector);


        }else if(strcmp(collector[0],"pause") == 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }

            free(buffer);
            free(collector);


        }else if(strcmp(collector[0],"resume") == 0){

            if(len == 2){
                printf("recognized:%s \n",buffer);
            }

            free(buffer);
            free(collector);


        }else if(strcmp(collector[0],"disable") == 0){

            if(len == 2){
                for(int i=0; i<printer ; i++){
                    if( strcmp(printers[i]->name,collector[1])){
                        printers[i]->enabled = 0;
                        printers[i]->busy = 0;
                        char buff[2048];
                        fprintf(stdout,"%s\n",imp_format_printer_status(printers[i],buff,2048));
                    }
                }
            }

            free(buffer);
            free(collector);



        }else if(strcmp(collector[0],"enable")== 0){

            if(len == 2){
                for(int i=0; i<printer ; i++){
                    if( strcmp(printers[i]->name,collector[1])){
                        printers[i]->enabled = 1;
                        printers[i]->busy = 0;
                        char buff[2048];
                        fprintf(stdout,"%s\n",imp_format_printer_status(printers[i],buff,2048));
                    }
                }
            }

            free(buffer);
            free(collector);

        }else{
            free(buffer);
            free(collector);
        }


        if(q->count >0){

            jobnode *jpt;
            for(jpt = q->front; jpt != NULL; jpt = jpt->next){

                if(jpt->j->status == QUEUED){
                    for(int i=0; i<printer; i++){

                        pathnode * p = findpath(g,jpt->j->file_type,printers[i]->type);
                        printpath(p);

                        if( printers[i]->enabled &&
                            !printers[i]->busy &&
                            (jpt->j->eligible_printers & (1<<printers[i]->id) ) != 0x0 &&
                            (strcmp(jpt->j->file_type,printers[i]->type)==0  || findpath(g,jpt->j->file_type,printers[i]->type) !=NULL)  ){

                            jpt->j->chosen_printer = printers[i];
                            jpt->j->status = RUNNING;
                            gettimeofday( &(jpt->j->change_time), NULL);
                            printers[i]->busy = 1;
                            char *job_update = malloc(1024);
                            imp_format_job_status(jpt->j, job_update, 1024);
                            printf("%s\n",job_update);
                            pathnode *p = findpath(g,jpt->j->file_type,printers[i]->type);


                            signal(SIGCHLD,handler);
                            pid_t pid = fork();
                            if(pid <0){
                                //error
                            }
                            if(pid == 0){

                                forkmaster(p, jpt);
                                sleep(1);
                                exit(0);

                            }


                        }
                    }
                }
            }
        }




    }



    exit(EXIT_SUCCESS);
}





void forkmaster(pathnode *path, jobnode *j){

    int stat;
    signal(SIGCHLD,handler);
    pathnode *ptr;
    if( setpgid(0,0) == -1 ){
        //error
    }

    int pipes[2];
    int pipes2[2];
    j->j->pgid = getpid();
     if( pipe(pipes) <0 || pipe(pipes2) <0  ){
         //error
     }

    pid_t cpid;



    for(ptr = path; ptr != NULL; ptr = ptr->next){
        cpid = fork();
        if(cpid <0){

        }

        if(cpid ==0){


            if( ptr->next == NULL){

                dup2(pipes[0],0);
                int prt  = imp_connect_to_printer(j->j->chosen_printer, PRINTER_NORMAL);
                dup2(prt,1);
                close(pipes[0]);
                close(pipes[1]);
                close(pipes2[0]);
                close(pipes2[1]);
                wait(&stat);
                exit(0);

            }else{
            dup2(pipes[0], 0);
            dup2(pipes2[1],1);
            close(pipes[0]);
            close(pipes[1]);
            close(pipes2[0]);
            close(pipes2[1]);
            char *cmd[] = { "/bin/cat", "./rsrc/a.txt", (char *)0 };
            execve (cmd[0], cmd, NULL);
            int r2 = pipes2[0];
            int w2 = pipes2[1];
            pipes2[0] = pipes[0];
            pipes2[1] = pipes[1];
            pipes[0] = r2;
            pipes[1] = w2;
            wait(&stat);
            exit(0);
        }


        }
    }


    close(pipes[0]);
    close(pipes[1]);
    close(pipes2[0]);
    close(pipes2[1]);




}



void handler(int sigint){
pid_t pid;
int status;


//get rid of terminated children
while((pid=waitpid(-1,&status,0)>0))
    ;

jobnode *j = findjobbyid(pid);
//for SIGTERM
if( WIFSIGNALED(status)){

    if(j != NULL){
        updatejobstat(j, ABORTED);
    }


}
//SIGSTOPPED
if( WIFSTOPPED(status)){

    if(j != NULL){
        updatejobstat(j, PAUSED);
    }

}
// FOR SIGCONT
else if( WIFCONTINUED(status)){

    if(j != NULL){
        updatejobstat(j, RUNNING);
    }


}
// check to see if the master exists
else if (WIFEXITED(status)){

    if(j != NULL){
        updatejobstat(j, COMPLETED);
    }


}
else if( !WIFEXITED(status)){

    if(j != NULL){
        updatejobstat(j, ABORTED);
    }


}
if(errno != ECHILD){

    perror("wait error!");

}

}




jobnode *findjobbyid(int id){

    jobnode * tmp;
    for(tmp = q->front; tmp != NULL; tmp = tmp->next){

        if(tmp->j->pgid == id){
            return tmp;
        }
    }

    return NULL;
}


void updatejobstat(jobnode *j, JOB_STATUS status){
    size_t size=100;
    char buf[size];
    char buf2[size];
    gettimeofday(&j->j->change_time,NULL);
    switch(status){

        case QUEUED:
        j->j->status = QUEUED;
        fprintf(stdout,"%s\n",imp_format_job_status(j->j,buf,size));
        break;

        case RUNNING:
        j->j->status = PAUSED;

        break;

        case ABORTED:
        j->j->status = ABORTED;
        j->j->chosen_printer->busy = 0;
        fprintf(stdout,"%s\n",imp_format_job_status(j->j,buf,size));
        fprintf(stdout,"%s\n",imp_format_printer_status(j->j->chosen_printer,buf2,size));

        break;

        case COMPLETED:
        j->j->status = COMPLETED;
        j->j->chosen_printer->busy = 0;
        j->j->chosen_printer->busy = 0;
        fprintf(stdout,"%s\n",imp_format_job_status(j->j,buf,size));
        fprintf(stdout,"%s\n",imp_format_printer_status(j->j->chosen_printer,buf2,size));

        break;

        case PAUSED:
        j->j->status = PAUSED;
        fprintf(stdout,"%s\n",imp_format_job_status(j->j,buf,size));
        break;


    }

}











