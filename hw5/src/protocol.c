#include "protocol.h"
#include "csapp.h"
#include <semaphore.h>
#include "debug.h"
#include "stdlib.h"
#include "stdio.h"
#include <inttypes.h>



// int readcnt = 0;
 //sem_t mutex;

int proto_send_packet(int fd, XACTO_PACKET *pkt, void *data){

     if(fd<3){
        perror("error occured");
        return -1;
    }

    pkt->size = htonl(pkt->size);
    pkt->timestamp_sec = htonl(pkt->timestamp_sec);
    pkt->timestamp_nsec = htonl(pkt->timestamp_nsec);

    if(  rio_writen(fd,pkt,sizeof(XACTO_PACKET)) <= 0){
        return -1;
    }

    if(pkt->size >0){

        if (rio_writen(fd, data, pkt->size) <= 0){
            return -1;
        }

    }


   //close(fd);
    return 0;

}



int proto_recv_packet(int fd, XACTO_PACKET *pkt, void **datap){

    if(fd<3){
        return -1;
    }


    int  count= 0;
    unsigned buff[sizeof(XACTO_PACKET)];

   if((count= rio_readn(fd,buff, sizeof(XACTO_PACKET))) <= 0){
        return -1;

   }

   memcpy(pkt, buff, sizeof(XACTO_PACKET));


    pkt->size = ntohl(pkt->size);
    pkt->timestamp_sec = ntohl(pkt->timestamp_sec);
    pkt->timestamp_nsec = ntohl(pkt->timestamp_nsec);

    if(pkt->size >0){

        void *ptr = malloc( sizeof(pkt->size));
        count = rio_readn(fd,ptr, pkt->size);

        if( count <= 0){
            //perror("error in receiving");
            //ptr = NULL;
            debug("EOF on fd: %d",fd);
            *datap = NULL;
            return  -1;
        }else{
            memcpy(datap, ptr, sizeof(XACTO_PACKET));

        }
    }else{
        *datap = NULL;
    }


    return 0;





}