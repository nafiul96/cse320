// #include "server.h"
// #include "csapp.h"
// #include "client_registry.h"
// #include "protocol.h"


// void *xacto_client_service(void *arg){

//     int connfd = *((int *)arg);
//     Free(arg);
//     Pthread_detach(pthread_self());

//     creg_register(client_registry, connfd);

//     while(1){

//         XACTO_PACKET *pkt = (XACTO_PACKET *)Malloc( sizeof (XACTO_PACKET));
//         void **data = Calloc(1,sizeof(int));
//         if( proto_recv_packet(connfd,pkt,data) <0){


//         }else{

//         }



//     }


// }











