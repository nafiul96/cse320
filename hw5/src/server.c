
#include "csapp.h"
#include "client_registry.h"
#include "protocol.h"
#include "transaction.h"
#include "protocol.h"
#include "debug.h"
#include "server.h"
#include <pthread.h>
#include "data.h"
#include "store.h"

/**
 * === DO NOT MODIFY THIS FILE ===
 * If you need some other prototypes or constants in a header, please put them
 * in another header file.
 *
 * When we grade, we will be replacing this file with our own copy.
 * You have been warned.
 * === DO NOT MODIFY THIS FILE ===
 */
//#ifndef SERVER_H
//#define SERVER_H


/*
 * Client registry that should be used to track the set of
 * file descriptors of currently connected clients.
 */
CLIENT_REGISTRY *client_registry;

/*
 * Thread function for the thread that handles client requests.
 *
 * @param  Pointer to a variable that holds the file descriptor for
 * the client connection.  This pointer must be freed once the file
 * descriptor has been retrieved.
 */
void *xacto_client_service(void *arg){

    int connfd = *((int *)arg);
    Free(arg);
    Pthread_detach(pthread_self());
    creg_register(client_registry, connfd);
    debug("Register client %d ",connfd);
    TRANSACTION *tp = trans_create();

    while(1){

        XACTO_PACKET *pkt = (XACTO_PACKET *)Malloc( sizeof(XACTO_PACKET));
        void **datap = Calloc(1,sizeof(int));
        if(proto_recv_packet(connfd, pkt, datap) == 0 ){

// XACTO_NO_PKT,  // Not used
//     XACTO_PUT_PKT, XACTO_GET_PKT, XACTO_DATA_PKT, XACTO_COMMIT_PKT,
//     XACTO_REPLY_PKT

            switch(pkt->type){

                case XACTO_PUT_PKT:
                //Packet reception
                debug("[%d] PUT packet received", connfd);

                // needs to retrieve two data packet(key, value)
                void **keydata, **content;


                keydata = Malloc( sizeof(void **) );
                if( proto_recv_packet(connfd,pkt,keydata) == 0){
                    debug("Received key, size %d",(int)strlen(*keydata));

                    content = Malloc( sizeof(int) );
                    if( proto_recv_packet(connfd, pkt, content) == 0 ){
                        debug("Received value, size %d",(int)strlen(*content));
                    }
                }

                //create key-blob and then create key with the key-blob
                BLOB *keyblob = blob_create(*keydata, strlen(*keydata));
                KEY *hkey = key_create(keyblob);
                //Free(keydata);
                //create val-blob
                BLOB *valblob = blob_create(*content, strlen(*content));
                //Free(content);
                store_put(tp, hkey, valblob);
                pkt->type = XACTO_REPLY_PKT;
                pkt->status = tp->status;
                pkt->size = 0;
                pkt->null = 0;
                proto_send_packet(connfd,pkt,NULL);
               // Free(pkt);
                break;



                case XACTO_GET_PKT:
                debug("[%d]Get packet received", connfd);
                void **kdata;
                kdata = Malloc( sizeof(void **) );
                if(  proto_recv_packet(connfd,pkt,kdata) == 0 ){
                    debug("Received key, size %d", (int)strlen(*kdata));
                    BLOB *kblob = blob_create(*kdata,  strlen(*kdata));
                    KEY *hskey = key_create(kblob);
                    BLOB **vblob = Malloc(sizeof(BLOB **));
                    pkt->status =  store_get(tp, hskey, vblob);
                    pkt->type = XACTO_REPLY_PKT;
                    pkt->size = 0;
                    pkt->null = 0;
                    proto_send_packet(connfd,pkt,NULL);
                    pkt->type = XACTO_DATA_PKT;
                    pkt->size = valblob->size;
                    pkt->null = 0;
                    proto_send_packet(connfd,pkt,valblob);
                }



                break;

                case XACTO_COMMIT_PKT:
                debug("Commit request has been received form %d", connfd);
                trans_commit(tp);
                pkt->type = XACTO_REPLY_PKT;
                pkt->status = tp->status;
                pkt->size = 0;
                pkt->null = 1;
                proto_send_packet(connfd,pkt,NULL);
                //Free(pkt);
                //Close(connfd);
                //return NULL;
                break;

            }


            //typedef enum { TRANS_PENDING, TRANS_COMMITTED, TRANS_ABORTED } TRANS_STATUS;
            store_show();
            trans_show(tp);
            if( tp->status== TRANS_ABORTED || tp->status == TRANS_COMMITTED){
                Close(connfd);
                break;
            }




        }


    }

    debug("Ending client service %d", connfd);
    creg_unregister(client_registry, connfd);
    debug("Unregister cleint %d", connfd);
    return NULL;
}

