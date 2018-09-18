#include <stdlib.h>

#include "debug.h"
#include "hw1.h"
#include "validhelp.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the content of three frames of audio data and
 * two annotation fields have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variables "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    unsigned long int mask;
    int argct = 1;

    if(argc<2){
        return  0;
    }


    argv++;



    if(isTop(*argv,'h')){
        debug("command h");
        mask = 0x8;
        mask = mask<<60;
        global_options |= mask;
        return  1;

    }else if(isTop(*argv,'u')){
        debug("command u");
        argct++;

        // sets the u bit
        mask  = 0x1;
        mask = mask<<62;
        global_options |= mask;


        if(argct<argc){
            argv++;
            argct++;

            if(isTop(*argv,'f')){
                argv++;
                argct++;

                if(validInt(*argv)== 0){
                    global_options = 0x0;
                    return 0;
                }

                mask = ctoi(*argv);
                mask--;
                if(mask<0 || mask > 1023){
                    global_options = 0x0;
                    return  0;
                }

                mask = mask<<48;

                global_options |= mask;


                if(argct<argc){

                    argct++;
                    argv++;

                    if(isTop(*argv,'p')){
                        mask = 0x1;
                        mask  = mask<<59;
                        global_options |=  mask;
                    }else{
                        global_options = 0x0;
                        return  0;
                    }
                }

                if(argct != argc){
                    global_options = 0x0;
                    return 0;
                }

                return  1;




            }else if(isTop(*argv, 'p')){
                mask = 0x1;
                mask = mask<<59;
                global_options |= mask;

                argv++;
                argct++;
                if(argct != argc){
                    global_options = 0x0;
                    return 0;
                }


            }else{
                global_options  = 0x0;

                return 0;

            }



        }




        return 1;

    }else if(isTop(*argv,'d')){
        debug("command d");
        argct++;

        // sets the u bit
        mask  = 0x1;
        mask = mask<<61;
        global_options |= mask;


        if(argct<argc){
            argv++;
            argct++;

            if(isTop(*argv,'f')){
                argv++;
                argct++;

                if(validInt(*argv)== 0){
                    global_options = 0x0;
                    return 0;
                }

                mask = ctoi(*argv);
                mask--;
                if(mask<0 || mask > 1023){
                    global_options = 0x0;
                    return  0;
                }

                mask = mask<<48;

                global_options |= mask;


                if(argct<argc){

                    argct++;
                    argv++;

                    if(isTop(*argv,'p')){
                        mask = 0x1;
                        mask  = mask<<59;
                        global_options |=  mask;
                    }else{
                        global_options = 0x0;
                        return  0;
                    }
                }

                if(argct != argc){
                    global_options = 0x0;
                    return 0;
                }

                return  1;




            }else if(isTop(*argv, 'p')){
                mask = 0x1;
                mask = mask<<59;
                global_options |= mask;

                argv++;
                argct++;
                if(argct != argc){
                    global_options = 0x0;
                    return 0;
                }


            }else{
                global_options  = 0x0;

                return 0;

            }



        }




        return 1;

    }else  if (isTop(*argv,'c')){
        debug("command c");
        argv++;
        argct++;

        mask  = 0x1;
        mask  = mask<<60;
        global_options |= mask;

        if(isTop(*argv,'k')){
            argv++;
            argct++;


            if(!validHexKey(*argv)){
                return 0;
            }


            global_options ^= ctoh(*argv);


            argct++;

            if(argct < argc){
                argv++;
                argct++;
                if(isTop(*argv,'p')){
                    mask  = 0x1;
                    mask = mask<<58;
                    global_options |= mask;

                    return 1;
                }
                return 0;
            }

            if(argct != argc){
                return 0;

            }


            return 1;
        }

        return 0;

    }else{

        return 0;
    }


}

/**
 * @brief  Recodes a Sun audio (.au) format audio stream, reading the stream
 * from standard input and writing the recoded stream to standard output.
 * @details  This function reads a sequence of bytes from the standard
 * input and interprets it as digital audio according to the Sun audio
 * (.au) format.  A selected transformation (determined by the global variable
 * "global_options") is applied to the audio stream and the transformed stream
 * is written to the standard output, again according to Sun audio format.
 *
 * @param  argv  Command-line arguments, for constructing modified annotation.
 * @return 1 if the recoding completed successfully, 0 otherwise.
 */
int recode(char **argv) {
    return 0;
}
