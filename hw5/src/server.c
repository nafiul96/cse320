
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
#include <time.h>


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

    XACTO_PACKET *rcv, *send;

    struct timespec clock;

    while(1){

        rcv = (XACTO_PACKET *)Calloc(1,sizeof(XACTO_PACKET));
        send = (XACTO_PACKET *)Calloc(1,sizeof(XACTO_PACKET));
        void **payload = Calloc( 1,sizeof(void **));

        if( proto_recv_packet(connfd, rcv,payload) < 0){
            if(tp->status == TRANS_PENDING){
                trans_abort(tp);
            }
            Free(rcv);
            Free(send);
            Free(payload);
            Close(connfd);
            break;

        }else{
            BLOB *hkey_blob, *value_blob;
            KEY *hkey;

            switch(rcv->type){

                case XACTO_PUT_PKT:
                debug("[%d] PUT packet received", connfd);

                // Receive Key
                proto_recv_packet(connfd,rcv,payload);
                //int ksize = rcv->size;
                debug("received key, size %d", rcv->size);
                hkey_blob = blob_create((char *)(*payload),rcv->size);
                hkey = key_create(hkey_blob);
                Free(*payload);

                // Receive Value
                proto_recv_packet(connfd,rcv,payload);
                // char *value = (char *)(*datap);
                // int vsize = rcv->size;
                debug("received value, size %d",rcv->size);
                value_blob = blob_create((char *)(*payload), rcv->size);
                Free(*payload);
                store_put(tp, hkey, value_blob);

                send->type = XACTO_REPLY_PKT;
                send->status = tp->status;
                send->size = 0;
                send->null = 0;
                //clock_gettime( CLOCK_REALTIME, &start)
                if(clock_gettime( CLOCK_MONOTONIC, &clock) == -1){
                    perror("error in getting time");
                }
                send->timestamp_sec = clock.tv_sec;
                send->timestamp_nsec = clock.tv_nsec;
                proto_send_packet(connfd,send,NULL);
                store_show();
                trans_show_all();
                break;


                case XACTO_GET_PKT:

                debug("[%d] GET packet received", connfd);

                proto_recv_packet(connfd,rcv, payload);
                debug("received key, size %d", rcv->size);
                hkey_blob = blob_create((char *)(*payload), rcv->size);
                Free(*payload);
                BLOB **bp = Calloc(1, sizeof(BLOB **) );
                hkey = key_create(hkey_blob);
                store_get(tp,hkey,bp);

                send->type = XACTO_REPLY_PKT;
                send->status = tp->status;
                send->size = 0;
                send->null = 0;
                if(clock_gettime( CLOCK_MONOTONIC, &clock) == -1 ){
                    perror("error in getting time");
                }

                send->timestamp_sec = clock.tv_sec;
                send->timestamp_nsec = clock.tv_nsec;
                proto_send_packet(connfd,send,NULL);

                send->type = XACTO_DATA_PKT;
                send->status = tp->status;
                send->size = 0;
                send->null = 0;
                if(clock_gettime( CLOCK_MONOTONIC, &clock) == -1 ){
                    perror("error in getting time");
                }

                send->timestamp_sec = clock.tv_sec;
                send->timestamp_nsec = clock.tv_nsec;
                if( *bp == NULL){
                    send->null = 1;
                    proto_send_packet(connfd,send,NULL);

                }else{
                    send->size = (*bp)->size;
                    proto_send_packet(connfd,send,(*bp)->content);
                    char why[]="unref copied version from getstore";
                    blob_unref(*bp,why);
                }

                Free(bp);
                store_show();
                trans_show_all();

                break;

                case XACTO_COMMIT_PKT:
                debug("[%d] COMMIT packet received", connfd);
                trans_commit(tp);
                send->type = XACTO_REPLY_PKT;
                send->status = tp->status;
                send->size = 0;
                send->null = 0;
                if( clock_gettime( CLOCK_MONOTONIC, &clock) ==-1){
                    perror("getting time error!");
                }

                send->timestamp_sec = clock.tv_sec;
                send->timestamp_nsec = clock.tv_nsec;
                proto_send_packet(connfd, send,NULL);

                store_show();
                trans_show_all();
                break;

            }

            Free(rcv);
            Free(send);
            Free(payload);

        }

        if( (tp->status == TRANS_ABORTED) || (tp->status == TRANS_COMMITTED) ){
            Close(connfd);
            break;

        }



    }

    debug("Ending client service %d", connfd);
    creg_unregister(client_registry, connfd);
    debug("Unregister cleint %d", connfd);
    return NULL;
}

