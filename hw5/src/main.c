#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "client_registry.h"
#include "transaction.h"
#include "store.h"
#include "server.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include "csapp.h"
#include <pthread.h>


static void terminate(int status);

void handle_hup(int status);


//int *fd;

CLIENT_REGISTRY *client_registry;

int main(int argc, char* argv[]){
    // Option processing should be performed here.
    // Option '-p <port>' is required in order to specify the port number
    // on which the server should listen.
    /**
     * Code for getting the port number
     */
    int opt, port;


    if(argc==1){
    fprintf(stderr, "usage: -p <port number>\n");
    exit(EXIT_FAILURE);
}


while((opt=getopt(argc, argv, "p:"))  != -1){

    switch(opt){

        case 'p':

            if(optarg == NULL){
                fprintf(stderr, "Usage: -p <port number>\n");
                exit(EXIT_FAILURE);
            }
            port = atoi(optarg);
            if(port <= 0){
                fprintf(stderr, "Usage: -p <port number>\n");
                exit(EXIT_FAILURE);
            }
        break;

        case'?':
            fprintf(stderr, "Usage: -p <port number>\n");
            exit(EXIT_FAILURE);

    }
}



    // Perform required initializations of the client_registry,
    // transaction manager, and object store.
    client_registry = creg_init();
    trans_init();
    store_init();

    // TODO: Set up the server socket and enter a loop to accept connections
    // on this socket.  For each connection, a thread should be started to
    // run function xacto_client_service().  In addition, you should install
    // a SIGHUP handler, so that receipt of SIGHUP will perform a clean
    // shutdown of the server.
    /**
     * The code for sighup
     */

    Signal(SIGHUP, handle_hup);

    int listenfd, *connfd;

    struct sockaddr_in clientaddr;
    pthread_t tid;
    listenfd = Open_listenfd(port);
    socklen_t clientlen = sizeof(struct sockaddr_in);

    while(1){

        //connfd=Malloc(sizeof(int));
        connfd = Malloc( sizeof(int));
        *connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        //open(*connfd);
        // fd = malloc( sizeof(int));
        // *fd = connfd;
        //creg_register(client_registry, *connfd);
        pthread_create(&tid,NULL,xacto_client_service, connfd);
    }




    fprintf(stderr, "You have to finish implementing main() "
	    "before the Xacto server will function.\n");

    terminate(EXIT_FAILURE);
}

/*
 * Function called to cleanly shut down the server.
 */
void terminate(int status) {
    // Shutdown all client connections.
    // This will trigger the eventual termination of service threads.
    creg_shutdown_all(client_registry);

    debug("Waiting for service threads to terminate...");
    creg_wait_for_empty(client_registry);
    debug("All service threads terminated.");

    // Finalize modules.
    creg_fini(client_registry);
    trans_fini();
    store_fini();

    debug("Xacto server terminating");
    exit(status);
}




void handle_hup(int status){

    //while((waitpid(-1,&status,0)>0));


    terminate(EXIT_SUCCESS);


}
