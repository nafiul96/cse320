
// #include "csapp.h"
// #include "client_registry.h"
// #include "protocol.h"
// #include "transaction.h"
// #include "protocol.h"
// #include "debug.h"
// #include "server.h"
// #include <pthread.h>

// /**
//  * === DO NOT MODIFY THIS FILE ===
//  * If you need some other prototypes or constants in a header, please put them
//  * in another header file.
//  *
//  * When we grade, we will be replacing this file with our own copy.
//  * You have been warned.
//  * === DO NOT MODIFY THIS FILE ===
//  */
// //#ifndef SERVER_H
// //#define SERVER_H


// /*
//  * Client registry that should be used to track the set of
//  * file descriptors of currently connected clients.
//  */
// CLIENT_REGISTRY *client_registry;

// /*
//  * Thread function for the thread that handles client requests.
//  *
//  * @param  Pointer to a variable that holds the file descriptor for
//  * the client connection.  This pointer must be freed once the file
//  * descriptor has been retrieved.
//  */
// void *xacto_client_service(void *arg){

//     int connfd = *((int *)arg);
//     Free(arg);
//     Pthread_detach(pthread_self());
//     debug("use %d", connfd);
//     fprintf(stderr, "%d\n",connfd );
//     creg_register(client_registry, connfd);
// //     //TRANSACTION *tp = trans_create();

// //     while(1){

// //         XACTO_PACKET *pkt = (XACTO_PACKET *)Malloc( sizeof(XACTO_PACKET));
// //         void **datap = Calloc(1,sizeof(int));
// //         if(proto_recv_packet(connfd, pkt, datap) >0 ){


// // // XACTO_NO_PKT,  // Not used
// // //     XACTO_PUT_PKT, XACTO_GET_PKT, XACTO_DATA_PKT, XACTO_COMMIT_PKT,
// // //     XACTO_REPLY_PKT

// //             switch(pkt->type){

// //                 case XACTO_PUT_PKT:
// //                 debug("Put request has been received from %d", connfd);
// //                 break;
// //                 case XACTO_GET_PKT:
// //                 debug("Get request has been received form %d", connfd);
// //                 break;
// //                 case XACTO_COMMIT_PKT:
// //                 debug("Commit request has been received form %d", connfd);
// //                 Close(connfd);
// //                 break;

// //             }


// //         }


// //     }



//     return NULL;
// }

// //#endif
