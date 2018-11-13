
#include "imprimer.h"


typedef struct jobnode{

JOB *j;
struct jobnode *next;
struct jobnode *prev;

}jobnode;






jobnode *front;
jobnode *rear;
int qsize;


// create the job queue
jobnode * createQ();

//append to the job Q
jobnode *insertjob(jobnode *j);

// deletes job
jobnode *rem_job(jobnode *j);

jobnode *remove_rear();
jobnode *removefromfront();

int jobExists(jobnode *j);

//find job by status
//job *findbyStat(char *stat);

//searchforjob
//job *searchforjob();

//search for



void printQ();

void printjobs();

