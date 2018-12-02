#include "server.h"
#include "csapp.h"
#include "client_registry.h"
#include "protocol.h"
#include "transaction.h"
#include "protocol.h"

void *xacto_client_service(void *arg){

    int connfd = *((int *)arg);
    Free(arg);
    Pthread_detach(pthread_self());

    creg_register(client_registry, connfd);
    //TRANSACTION *trans = trans_create();

 // XACTO_NO_PKT,  // Not used
 //    XACTO_PUT_PKT, XACTO_GET_PKT, XACTO_DATA_PKT, XACTO_COMMIT_PKT,
 //    XACTO_REPLY_PKT

    while(1){

        XACTO_PACKET *pkt = (XACTO_PACKET *)Malloc( sizeof (XACTO_PACKET));
        void **data = Calloc(1,sizeof(int));
        if( proto_recv_packet(connfd,pkt,data) >0){

            // switch(pkt->type){
            //     XACTO_PUT_PKT:
            //     debug("[%d] PUT packet received",connfd);
            //     //debug("[%d] PUT packet received",connfd);
            //     //debug("[%d] PUT packet received",connfd);


            //     break;
            //     XACTO_GET_PKT:
            //     break;
            //     XACTO_COMMIT_PKT:
            //     break;

            // }


        }



    }


}











