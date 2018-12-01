// #include <stdlib.h>
// #include "client_registry.h"
// #include "stdio.h"
// #include <semaphore.h>
// #include "csapp.h"


// #define MAX_CONN 1024

// typedef struct client_registry{

//     int *buf;
//     int n;
//     int front;
//     int rear;
//     sem_t mutex;
//     sem_t slots;
//     sem_t items;

// }client_registry;


// // //  * Initialize a new client registry.
// // //  *
// // //  * @return  the newly initialized client registry.

// CLIENT_REGISTRY *creg_init(){

//     CLIENT_REGISTRY *temp = Malloc(sizeof(client_registry));
//     temp->buf = Calloc(MAX_CONN,sizeof(int));
//     temp->n = MAX_CONN;
//     temp->front = temp->rear = 0;
//     Sem_init(&temp->mutex,0,1);
//     Sem_init(&temp->slots,0,MAX_CONN);
//     Sem_init(&temp->items,0,0);

//     return temp;

// }


// void creg_fini(CLIENT_REGISTRY *cr){

//     free(cr);
// }


// void creg_register(CLIENT_REGISTRY *cr, int fd){

//     P(&cr->slots);
//     P(&cr->mutex);
//     cr->buf[(++cr->rear)%(cr->n)] = fd;
//     for(int i= cr->front; i != cr->rear; ((++i)/(cr->n) ) ){

//     }


//     V(&cr->mutex);
//     V(&cr->slots);

// }


// void creg_unregister(CLIENT_REGISTRY *cr, int fd){

//     P(&cr->items);
//     P(&cr->mutex);
//     // cr->buf[((++cr->front)%(cr->n))];
//     cr->buf[fd] = 0;
//     V(&cr->mutex);
//     V(&cr->slots);
// }

// void creg_wait_for_empty(CLIENT_REGISTRY *cr){

//     P(&cr->items);
// }
