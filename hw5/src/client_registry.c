#include <stdlib.h>
#include "client_registry.h"
#include "stdio.h"
#include "debug.h"
#include <semaphore.h>
#include "csapp.h"


#define MAX_CONN 1024

typedef struct client_registry{

    int buf[MAX_CONN]; // to hold fds
    int n; // current number of fds
    sem_t mutex; // for locking
    sem_t slots; // for blocking the slot
    sem_t items; // for blocking the items

}client_registry;




CLIENT_REGISTRY *creg_init(){

    CLIENT_REGISTRY *temp = Malloc(sizeof(client_registry));
    for(int i=0; i< MAX_CONN; i++){
        temp->buf[i] = 0;
    }
    temp->n = 0;
    Sem_init(&temp->mutex,0,1);
    Sem_init(&temp->slots,0,MAX_CONN);
    Sem_init(&temp->items,0,0);
    debug("Initialize client registry");
    return temp;

}


void creg_fini(CLIENT_REGISTRY *cr){

    Free(cr);
}


void creg_register(CLIENT_REGISTRY *cr, int fd){

    P(&cr->slots);
    P(&cr->mutex);
    if(cr->n == MAX_CONN){
        debug("server has reached maximum client_registry ");
    }

    for(int i=0; i<MAX_CONN; i++){
        if(cr->buf[i] == 0 ){

            cr->buf[i] = fd;
            cr->n = cr->n + 1;

            break;
        }
    }

    V(&cr->mutex);
    V(&cr->items);

}


void creg_unregister(CLIENT_REGISTRY *cr, int fd){

    P(&cr->items);
    P(&cr->mutex);

    if(cr->n == 0){
        debug("cleints buffer is empty");
        return;
    }
   for(int i=0; i< MAX_CONN; i++){
        if(cr->buf[i] == fd){
            cr->buf[i] = 0;
            cr->n = cr->n -1;
            debug("Unregister client %d (total connected: %d)",fd, cr->n);
            break;
        }
   }


    V(&cr->mutex);
    V(&cr->slots);
}

void creg_wait_for_empty(CLIENT_REGISTRY *cr){
    // P(&cr->mutex);
    // for(int i=0; i<1024; i++){

    // }
    // V(&cr->mutex);
    //P(&cr->items);
    //
    // while(cr->n >0){
    //     creg_unregister();
    // }
    for(int i=0 ; i<MAX_CONN; i++){
        if(cr->buf[i] != 0){
            creg_unregister(cr,cr->buf[i]);
        }
    }
}

void creg_shutdown_all(CLIENT_REGISTRY *cr){
    P(&cr->mutex);
    for(int i=0; i<MAX_CONN; i++){
        if(cr->buf[i] !=0){
            shutdown(cr->buf[i], SHUT_RD);
            //creg_unregister(cr,cr->buf[i]);
        }
    }

    V(&cr->mutex);


}