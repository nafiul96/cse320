#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    //double* ptr = sf_malloc(sizeof(double));
    double* ptr = sf_malloc(4096);
    sf_malloc(-1);

    *ptr = 320320320e-320;

     sf_malloc(55555);


    printf("%f\n", *ptr);


    sf_free(ptr);

    // printf("%x\n",sf_mem_start);
    // printf("%x\n",sf_mem_end);
    //int* bp = sf_mem_grow();
    //printf("%d\n", *bp );

    //sf_mem_fini();



    return EXIT_SUCCESS;
}
