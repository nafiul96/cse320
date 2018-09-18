
int isTop(char *cha, char key){

    int size = 0;
    char *p = cha;
    while(*p != 0){
        p++;
        size++;
    }

    if(size != 2){
        return 0;
    }

    char *ptr = cha;
    if(*ptr == '-'){
        ptr++;
        if(*ptr == key){
            return 1;
        }
    }

    return  0;
}

int validHexKey(char *key){

    char *ptr = key;
    int size = 0;

    while(*ptr != 0){
        size++;
        ptr++;
    }

    if(size<0 || size >8){
        return 0;
    }


    char *p  = key;

    while(*p != 0){
        if(  (*p >='0' && *p<= '9') || (*p >='A' && *p<= 'F') || (*p >='a' && *p<= 'f') ){
            p++;

        }else{
            return 0;
        }
    }

    return 1;
}




unsigned long int ctoh(char *c){


    int i = 1;
    unsigned long int ctoh = 0x0;

    while(*c !=0){
        ctoh = ctoh<<4;
        if(*c>= '0' && *c<= '9'){
            ctoh |=  (*c-'0');

        }else if(*c>='a' && *c<= 'f'){
            ctoh |= (*c-'a'+ 0xa);

        }else if(*c>= 'A' && *c<='F'){
            ctoh |= (*c-'A'+ 0xa);
        }
        c++;
        i++;
    }

    return ctoh;
}


int validInt(char *c){

    while(*c != 0){
        if(*c >= '0' && *c <= '9'){
            c++;
        }else{
            return 0;
        }
    }

    return 1;
}


int mathpow(int a, int b){
    unsigned int sum = 1;

    int i;

    for(i=0; i<b; i++){
        sum *= a;
    }



    return sum;
}


int countSize(char *c){

    int size =0;
    while(*c != 0){
        size++;
    }

    return size;
}


unsigned int ctoi(char *c){

    char *p = c;
    unsigned int size =0;
    unsigned int sum = 0;

    while(*p != 0){
        size++;
        p++;
    }

    char *t = c;
    while(*t != 0){
        size--;
        sum += (*t - '0') * mathpow(10,size);
        t++;
    }

    return sum;

}












