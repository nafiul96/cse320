#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
     sf_mem_init();

    //double* ptr = sf_malloc(sizeof(double));

    // *ptr = 320320320e-320;



     //printf("%f\n", *ptr);


     //sf_free(ptr);


     // void *ptr[8];

     // for(int i=0; i<8; i++){
     //    ptr[i] = sf_malloc(sizeof(double)); //32
     // }

     // sf_free(ptr[2]);


     // sf_free(ptr[4]);


     // sf_free(ptr[5]);


     // ptr[5] = sf_realloc(ptr[3],1024);

     void *ptr[6];

     for(int i=0; i<6; i++){
        ptr[i] = sf_malloc(80);
     }

     for(int i=0; i<6; i++){
        ptr[i] = sf_realloc(ptr[i],50);
     }

     for(int i=0; i<6;i++){

        if(i%2 == 0){
            sf_free(ptr[i]);
        }
     }



     sf_show_heap();





    sf_mem_fini();



    return EXIT_SUCCESS;
}
