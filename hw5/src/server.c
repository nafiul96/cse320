
// #include "csapp.h"
// #include "client_registry.h"
// #include "protocol.h"
// #include "transaction.h"
// #include "protocol.h"
// #include "debug.h"
// #include "server.h"
// #include <pthread.h>
// #include "data.h"
// #include "store.h"
// #include <time.h>

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
//     creg_register(client_registry, connfd);
//     debug("Register client %d ",connfd);
//     TRANSACTION *tp = trans_create();

//     XACTO_PACKET *rcv, *send;

//     struct timeval clock;
//     double sec;
//     double nsec;

//     while(1){

//         rcv = (XACTO_PACKET *)Malloc(sizeof(XACTO_PACKET));
//         send = (XACTO_PACKET *)Malloc(sizeof(XACTO_PACKET));
//         void **datap = Malloc( sizeof(void **));

//         if( proto_recv_packet(connfd, rcv,datap) == -1){
//             Free(rcv);
//             Free(send);
//             Free(datap);
//             Close(connfd);
//             break;

//         }else{


//             BLOB *key_blob, *value_blob;
//             KEY *hkey;

//             switch(rcv->type){

//                 case XACTO_PUT_PKT:
//                 debug("[%d] PUT packet received", connfd);
//                 proto_recv_packet(connfd,rcv,datap);
//                 char *key = *datap;
//                 int ksize = strlen(key);
//                 debug("received key, size %d", ksize);
//                 //Free(key);
//                 proto_recv_packet(connfd,rcv,datap);
//                 char *value = *datap;
//                 int vsize = strlen(value);
//                 debug("received value, size %d",vsize);
//                 //Free(value);
//                 key_blob = blob_create(key,ksize);
//                 hkey = key_create(key_blob);
//                 value_blob = blob_create(value, vsize);
//                 store_put(tp, hkey, value_blob);
//                 send->type = XACTO_REPLY_PKT;
//                 send->status = tp->status;
//                 send->size = 0;
//                 send->null = 0;
//                 if(gettimeofday(&clock, NULL) == -1){
//                     perror("error in getting time");
//                 }
//                 sec = (double)clock.tv_sec;
//                 nsec = (double)clock.tv_usec;
//                 send->timestamp_sec = sec;
//                 send->timestamp_nsec = nsec*1000;
//                 proto_send_packet(connfd,send,NULL);
//                 store_show();
//                 trans_show_all();
//                 break;

//                 case XACTO_GET_PKT:
//                 debug("[%d] GET packet received", connfd);
//                 char *getkey;
//                 proto_recv_packet(connfd,rcv, datap);
//                 getkey = *datap;
//                 int keylen = strlen(getkey);
//                 debug("received key, size %d", keylen);
//                 key_blob = blob_create(getkey, keylen);
//                 //debug("[%d] Value is %s", connfd, bp->prefix);
//                 //Free(getkey);
//                 BLOB **bp = Malloc( sizeof(BLOB **) );
//                 hkey = key_create(key_blob);
//                 store_get(tp,hkey,bp);

//                 send->type = XACTO_REPLY_PKT;
//                 send->status = tp->status;
//                 send->size = 0;
//                 send->null = 0;
//                 if(gettimeofday(&clock, NULL) == -1 ){
//                     perror("error in getting time");
//                 }
//                 sec = (double)clock.tv_sec;
//                 nsec = (double)clock.tv_usec;
//                 send->timestamp_sec = sec;
//                 send->timestamp_nsec = nsec * 1000;
//                 proto_send_packet(connfd,send,NULL);

//                 send->type = XACTO_DATA_PKT;
//                 send->status = tp->status;
//                 send->size = 0;
//                 send->null = 0;
//                 if(gettimeofday(&clock, NULL) == -1 ){
//                     perror("error in getting time");
//                 }
//                 sec = (double)clock.tv_sec;
//                 nsec = (double)clock.tv_usec;
//                 send->timestamp_sec = sec;
//                 send->timestamp_nsec = nsec * 1000;
//                 if( *bp == NULL){
//                     send->null = 1;
//                     proto_send_packet(connfd,send,NULL);

//                 }else{
//                     send->size = strlen((*bp)->content);
//                     proto_send_packet(connfd,send,(*bp)->content);
//                     char why[]="gettig from store";
//                     blob_unref(*bp, why);
//                 }


//                 store_show();
//                 trans_show_all();

//                 break;

//                 case XACTO_COMMIT_PKT:
//                 debug("[%d] COMMIT packet received", connfd);
//                 trans_commit(tp);
//                 send->type = XACTO_REPLY_PKT;
//                 send->status = tp->status;
//                 send->size = 0;
//                 send->null = 0;
//                 if( gettimeofday(&clock, NULL) ==-1){
//                     perror("getting time error!");
//                 }
//                 sec= (double)clock.tv_sec;
//                 nsec = (double)clock.tv_usec;
//                 send->timestamp_sec = sec;
//                 send->timestamp_nsec = nsec * 1000;
//                 // if(!send->null){
//                 //     proto_send_packet(connfd, send,(*bp)->content);
//                 // }else{
//                     proto_send_packet(connfd, send,NULL);
//                // }

//                 store_show();
//                 trans_show_all();
//                 break;

//             }



//             Free(rcv);
//             Free(send);
//             Free(datap);

//         }

//         if( (tp->status == TRANS_ABORTED) || (tp->status == TRANS_COMMITTED) ){
//             Close(connfd);
//             break;

//         }



//     }

//     debug("Ending client service %d", connfd);
//     creg_unregister(client_registry, connfd);
//     debug("Unregister cleint %d", connfd);
//     return NULL;
// }

