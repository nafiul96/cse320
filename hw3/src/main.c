#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
     sf_mem_init();

    // double* ptr = sf_malloc(sizeof(double));

     //*ptr = 320320320e-320;



     //printf("%f\n", *ptr);


     //sf_free(ptr);

     void *ptr[10];

     for(int i=0; i<10; i++){
        ptr[i] = sf_malloc(80);
     }


     for(int i=0; i<10; i++){
        ptr[i] = sf_realloc(ptr[i],50);
     }


     for(int i=0; i<10;i++){

        if(i%4 == 0){
            sf_free(ptr[i]);
        }
     }


     sf_show_heap();
     //sf_free(ptr[0]);
    sf_mem_fini();



    return EXIT_SUCCESS;
}
