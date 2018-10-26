#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {
     sf_mem_init();

     double* ptr = sf_malloc(sizeof(double));

    // double* ptr = sf_malloc(4096);
    // sf_malloc(-1);

     *ptr = 320320320e-320;

    //  sf_malloc(55555);


     printf("%f\n", *ptr);


     sf_free(ptr);

    // double* ptr = sf_malloc(sizeof(double)); //32

   // // /* void *y = */ sf_malloc(10);    //32
   //  //x = sf_realloc(x, sizeof(int) * 10);

    //sf_free(ptr);


    // printf("%x\n",sf_mem_start);
    // printf("%x\n",sf_mem_end);
    //int* bp = sf_mem_grow();
    //printf("%d\n", *bp );


    //  /* void *w = */ sf_malloc(sizeof(long));
    // void *x = sf_malloc(sizeof(double) * 11);
    // void *y = sf_malloc(sizeof(char));
    // /* void *z = */ sf_malloc(sizeof(int));

    // sf_free(y);
    // sf_free(x);

    // sf_show_heap();

    //  void *u = sf_malloc(4096); //32


    // sf_free(u);


    // /* void *v = */ sf_malloc(4096); //


    // void *w = sf_malloc(100); //


    // w = sf_realloc(w, 124);

    // w= sf_realloc(w,4);


    // sf_free(w);




     // for(int i=0; i<50; i++){

     //    if(i%3==0 && i%5==0){
     //        if(i<50){
     //            ptr[i] = sf_realloc(ptr[i],10);

     //        }else{

     //            ptr[i] = sf_realloc(ptr[i],112);

     //        }
     //    }else if(i%3 ==0){
     //        sf_free(ptr[i]);

     //    }else if(i%5){
     //        sf_free(ptr[i]);
     //    }
     // }

     // void *ptr[8];

     // for(int i=0; i<8; i++){
     //    ptr[i] = sf_malloc(sizeof(double)); //32
     // }

      //sf_free(ptr[0]);

     // sf_free(ptr[4]);

     // sf_free(ptr[5]);

     // ptr[5] = sf_realloc(ptr[3],1024);
    //sf_malloc(sizeof(double));


    //sf_free(ptr[0]);

     // void *ptr[10];

     // for(int i=0; i<10; i++){
     //    ptr[i] = sf_malloc(80);
     // }

     // for(int i=0; i<10; i++){
     //    ptr[i] = sf_realloc(ptr[i],50);
     // }

     // for(int i=0; i<10;i++){

     //    if(i%4 == 0){
     //        sf_free(ptr[i]);
     //    }
     // }



   // sf_show_heap();






    sf_mem_fini();



    return EXIT_SUCCESS;
}
