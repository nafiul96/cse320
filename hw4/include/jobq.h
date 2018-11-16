#include "imprimer.h"

typedef struct jobnode
{
    JOB * j;
    struct jobnode *next;
    struct jobnode *prev;

}jobnode;


typedef struct jobq
{
    jobnode *front;
    jobnode *rear;
    int count;
}jobq;



jobq *q;

jobq *createQ();

jobnode * enqueue(jobq *q, jobnode *j);

jobnode *dequeue(jobq * q);


void printQ(jobq *q);

