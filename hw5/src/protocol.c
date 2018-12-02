// #include "protocol.h"
// #include "csapp.h"
// #include <semaphore.h>
// #include "debug.h"
// #include "stdlib.h"
// #include "stdio.h"
// #include <inttypes.h>


// //static void init_echo_cnt(void);
// // int readcnt = 0;
// //static sem_t mutex;
// //static int count;

// int proto_send_packet(int fd, XACTO_PACKET *pkt, void *data){


//     int size = pkt->size;
//     pkt->size = htonl(pkt->size);
//     pkt->timestamp_sec = htonl(pkt->timestamp_sec);
//     pkt->timestamp_nsec = htonl(pkt->timestamp_nsec);
//     if(    rio_writen(fd,pkt,sizeof(XACTO_PACKET)) < 0){
//         return -1;
//     }
//     if(size >0){
//         if ( rio_writen(fd, data, pkt->size) < 0){
//             return -1;
//         }

//     }

//     return 0;

// }



// int proto_recv_packet(int fd, XACTO_PACKET *pkt, void **datap){

//     if( rio_readn(fd,pkt, sizeof(XACTO_PACKET)) <= 0 ){
//         return -1;
//     }

//     //int size = pkt->size;
//     pkt->size = ntohl(pkt->size);
//     pkt->timestamp_sec = ntohl(pkt->timestamp_sec);
//     pkt->timestamp_nsec = ntohl(pkt->timestamp_nsec);

//     if( pkt->size >0){
//         *datap = malloc(  sizeof(pkt->size));
//         if( rio_readn(fd, *datap, pkt->size) <= 0 ){
//             return -1;
//         }
//     }
//     return 0;


// }