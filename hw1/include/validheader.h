

unsigned int readstrm(unsigned int size){


  int i=0;
  unsigned int ct = 0x0;
  unsigned int mask = 0x0;
  while(i<size){
    ct = getchar();

    if(ct != ' ' && ct != '\n'){

      mask = mask<<8;
      mask |= ct;
      i++;

    }
  }

  return mask;

}


int read_header(AUDIO_HEADER *hp){

  hp->magic_number = readstrm(4);
  hp->data_offset = readstrm(4);
  hp-> data_size = readstrm(4);
  hp->encoding = readstrm(4);
  hp-> sample_rate = readstrm(4);
  hp->channels = readstrm(4);


  if(hp->magic_number != AUDIO_MAGIC){
    return 0;
  }

  if(hp->data_offset%8 != 0){
    return 0;
  }

  if(hp->encoding < 2 || hp->encoding >5){
    return  0;
  }

    unsigned int annotation_size = hp->data_offset - 24;
    if(annotation_size>0){

        if(annotation_size>ANNOTATION_MAX){

          return 0;
        }
     }


  return 1;
}




int write_header(AUDIO_HEADER *hp){



  char* p = (char*)&hp->magic_number;
  p += 3;
  int i = 0;
  while(i <4){
    putchar(*p);
    p--;
    i++;
   }


    p=(char*)&hp->data_offset;
    p += 3;
    i = 0;
    while(i < 4){
      putchar(*p);
      p--;
      i++;
   }


  p=(char*)&hp->data_size;
  p += 3;
  i = 0;
  while(i < 4){
    putchar(*p);
    p--;
    i++;
   }


  p = (char*)&hp->encoding;
  p += 3;
  i = 0;
  while(i < 4){
   putchar(*p);
    p--;
    i++;
   }

  p = (char*)&hp->sample_rate;
  p += 3;
  i = 0;
  while(i < 4){
    putchar(*p);
    p--;
    i++;
   }

  p = (char*)&hp->channels;
  p += 3;
  i = 0;
  while(i < 4){
    putchar(*p);
    p--;
    i++;
  }


    return 1;
}


int read_annotation(char *ap, unsigned int size){

    if(size > ANNOTATION_MAX){
      return 0;
    }
    int i =0;
    char c = 0;
    while(i<size){

      c = getchar();
      if(c != ' ' || c != '\n'){
        *ap = c;
        if(i == (size-1)){
          if(*ap != '\0'){
            return 0;
          }
        }
        i++;
        ap++;
      }

    }

   return 1;


}

int write_annotation(char *ap, unsigned int size){

   if(size> ANNOTATION_MAX){
    return 0;
   }


  int i = 0;
  while(i< size){

    putchar(*ap);
    ap++;
    i++;
  }


   return 1;
}









int read_frame(int *fp, int channels, int bytes_per_sample){

   int size = channels*bytes_per_sample;
   int c = 0;
   while(size>0 && (c=getchar()) != EOF){
     *fp = *fp<<8;
    *fp |= c;
    size--;
   }

  if(c == EOF){
    return 0;
}

  return 1;
}



int write_frame(int *fp, int channels, int bytes_per_sample){

  int size =channels*bytes_per_sample;
  char* p;
  p = (char*)&(*fp);
  p +=(size-1);
   while(size > 0){
    putchar(*p);
    p--;
    size--;
   }


   if(size !=0){

    return 0;

   }


    return 1;
}


void speedUp(AUDIO_HEADER *hp, int factor){

    int bps = hp->encoding-1;
    int frames = hp->data_size/bps;
    int i;
    for(i=0; i<frames; i++){

      read_frame((int*)input_frame,hp->channels, bps);
      if((i%factor)==0){
          write_frame((int*)input_frame, hp->channels, bps);
      }else{
        hp->data_size = hp->data_size -1;
      }

    }
}




void slowDown(AUDIO_HEADER *hp, int factor){

  int fct = 0;
  int i = 0;
  int frames = hp->data_size/ (hp->encoding-1);
  int bps = hp->encoding-1;

  if(read_frame((int*)previous_frame,hp->channels,bps) ==0){
    return;
  }

  write_frame((int*)previous_frame,hp->channels,bps);

  for(fct=0; fct<frames-1; fct++){

    read_frame((int*)input_frame,hp->channels,bps);

    char* ot = previous_frame;
    char* os = input_frame;
    char* p = output_frame;

    for(i = 0; i<factor-1; i++){


      while(*ot != 0 && *os != 0){
        *p = *ot + (*os- *ot)*i/factor;
        ot++;
        os++;
        p++;
    }

      write_frame((int*)output_frame, hp->channels, bps);
      hp->data_size = hp->data_size +1;
    }

    write_frame((int*)input_frame, hp->channels, bps);

    char *pp = previous_frame;
    char *ip = input_frame;
    while(*ip !=0){
      *pp = *ip;
      pp++;
      ip++;
    }
    if(read_frame((int*)previous_frame,hp->channels,bps) == EOF){

      return ;

    }

  }
}




void cipherifyHeader(AUDIO_HEADER *hp){

  hp->magic_number  ^= myrand32();
  hp->data_offset  ^= myrand32();
  hp->data_size  ^= myrand32();
  hp->encoding  ^= myrand32();
  hp->sample_rate  ^= myrand32();
  hp->channels ^= myrand32();

}



void encryptframe(char *c, int size){

  int  i=0;
  while(i<size){
    *c ^= myrand32();
    c++;
    i++;
  }
}



void cipherifyData(AUDIO_HEADER *hp){

  int  i = 0;
  int bps = hp->encoding-1;
  int  size  = bps * hp->channels;
  int frames = hp->data_size/ bps;
  char *p  = 0;

  while(i < frames){
    read_frame((int *)input_frame,hp->channels, bps);
    encryptframe(input_frame,size);
    write_frame((int *)input_frame,hp->channels,bps);
    i++;
  }
}


