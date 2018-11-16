#include <stdlib.h>
#include <stdio.h>
#include "imprimer.h"
#include "jobq.h"

jobq *createQ(){


    jobq *temp =  malloc( sizeof(jobq)   );
    temp->front = NULL;
    temp->rear = temp->front;
    temp->count = 0;
    return temp;

}

jobnode * enqueue(jobq * q,jobnode *j){

    if(q->count == 0){
        q->front = j;
        q->rear = q->front;
        q->rear->next  = NULL;
        q->count = q->count +1;
        return q->rear;

    }else{

        q->rear->next = j;
        j->prev = q->rear;
        j->next = NULL;
        q->rear = j;
        q->count = q->count +1;
        return q->rear;

    }
}

jobnode *dequeue(jobq * q){

    if(q->count <= 0){
        return NULL;
    }else{
        jobnode * j = q->front;
        q->front =  q->front->next;
        j->next = NULL;
        j->prev = NULL;
        return j;
    }


}


void printQ(jobq *q){

    jobnode *tmp ;

    for(tmp = q->front; tmp != NULL;tmp = tmp->next){


        char *buffer  =  malloc( 4096  );

        imp_format_job_status(tmp->j, buffer, 4096);
        printf("%s\n",buffer);
    }
}

















