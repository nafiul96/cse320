#include <stdlib.h>
#include <stdio.h>
#include "queue.h"



jobnode *front;
jobnode *rear;
int qsize;

// create the job queue
jobnode * createQ(){

    if ( (front = (jobnode *)malloc(sizeof(jobnode)) ) == NULL){
        fprintf(stderr,"%s", "error ouccred");
    }
    rear = front;
    qsize = 0;

    return front;
}


//append to the job Q
jobnode *insertjob(jobnode *j){

    jobnode * node = j;
    node->next = front;
    node->prev = rear;
    front->prev = node;
    rear->next = node;
    rear = node;
    qsize ++;
    return rear;
}

// deletes job
jobnode *rem_job(jobnode *j){

    if(jobExists(j)){
        jobnode *temp = j;
        temp->prev = temp->next;
        temp->next->prev = temp->prev;
        qsize--;
        return temp;
    }

    return NULL;

}


int jobExists(jobnode *j){
    jobnode *temp = front;
    for(temp = front->next; temp != front; temp =temp->next){
        if(temp == j){
            return 1;
        }
    }

    return 0;
}


jobnode *remove_rear(){
    if(qsize<=0){
        return NULL;
    }
    else{

        jobnode *temp = rear;
        front->prev = rear->prev;
        rear->next = front;
        qsize--;
        return temp;

    }

}


//find job by status
//job *findbyStat(char *stat);

//searchforjob
//job *searchforjob();

//search for

jobnode *removefromfront(){
    if(qsize <= 0 ){
        return NULL;
    }
    jobnode *temp = front->next;
    front->next = temp->next;
    temp->next->prev = front;
    return temp;
}



void printQ(){

    jobnode *j = NULL;

    if(qsize==0){
        printf("Sorry! The Q is empty\n");
    }else{

        for(j = front->next; j != front; j = j->next){

           // printf("[%d   %s] [p:%p, n:%p]\n",j->id,j->data,j->prev,j->next);
            //char *imp_format_job_status(JOB *job, char *buf, size_t size);

            char *buff = malloc(4048);
            imp_format_job_status(j->j, buff, 4048);
            printf("%s\n",buff);


        }
    }

}


void printjobs(){

    jobnode *j = NULL;

    if(qsize==0){
        printf("Sorry! The Q is empty\n");
    }else{

        for(j = front->next; j != front; j = j->next){

            char *buff = malloc(8192);
            imp_format_job_status(j->j, buff, 8192);
            printf("%s\n",buff);
            free(buff);


        }
    }
}


