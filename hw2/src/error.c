/*
 * Error handling routines
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"


void fatal(char *fmt, ...)
{

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(1);
}

// void error(char *fmt, ...)
// {
//         fprintf(stderr, "\nError: ");
//         fprintf(stderr, fmt, a1, a2, a3, a4, a5, a6);
//         fprintf(stderr, "\n");
//         errors++;
// }




void error(char *fmt, ...){


    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    errors++;
}

// void warning(char *fmt, char *a1, char *a2, char *a3, char *a4, char *a5, char *a6)

// {
//         fprintf(stderr, "\nWarning: ");
//         fprintf(stderr, fmt, a1, a2, a3, a4, a5, a6);
//         fprintf(stderr, "\n");
//         warnings++;
// }

void warning(char *fmt, ...){

    // va_list ap;
    // char *s ;
    // va_start(ap, fmt);
    // while(*fmt++){
    //     s = va_arg(ap, char*);
    //     //fprintf(stderr, "\nWarning");
    //     fprintf(stderr,"%s",s);
    //     fprintf(stderr, "\n");
    // }
    // va_end(ap);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    warnings++;


}






// void debug(char *fmt, char *a1, char *a2, char *a3, char *a4, char *a5, char *a6)
// {
//         if(!dbflag) return;
//         fprintf(stderr, "\nDebug: ");
//         fprintf(stderr, fmt, a1, a2, a3, a4, a5, a6);
//         fprintf(stderr, "\n");
// }


void debug(char *fmt, ...)
{
        if(!dbflag) return;
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        fprintf(stderr, "\n");
        va_end(args);
}