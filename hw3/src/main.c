#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
     sf_mem_init();

    // //double* ptr = sf_malloc(sizeof(double));
    // double* ptr = sf_malloc(4096);
    // sf_malloc(-1);

    // *ptr = 320320320e-320;

    //  sf_malloc(55555);


    // printf("%f\n", *ptr);


    // sf_free(ptr);

     void *u = sf_malloc(1); //32
    /* void *v = */ sf_malloc(40); //48
    void *w = sf_malloc(152); //160
    /* void *x = */ sf_malloc(536); //544
    void *y = sf_malloc(1); // 32
    /* void *z = */ sf_malloc(2072); //2080

    sf_free(u);
    sf_free(w);
    sf_free(y);


    // printf("%x\n",sf_mem_start);
    // printf("%x\n",sf_mem_end);
    //int* bp = sf_mem_grow();
    //printf("%d\n", *bp );

    //sf_mem_fini();



    return EXIT_SUCCESS;
}
