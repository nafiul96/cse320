/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"
#include<errno.h>




void *find_bestfit(size_t size);
void placeIt(void* bp, size_t asize, size_t size);
void *coalesce(void *bp);
void remove_from_blocklist(sf_header *blockptr);
void pushblock(sf_header *head, sf_header *node);
void insert_to_list(sf_free_list_node *node, sf_header *block);


void *sf_malloc(size_t size) {


    // the adjusted size of the request
    size_t asize;
    void* bp;

    // check to see if the size is zero
    if(size <= 0) return  NULL;


    //asize  =  size + sizeof(sf_header);

    asize  =  size + sizeof(sf_block_info);


    //check to see if the size is a multiple of 16
    if(asize%16 != 0)
        asize  = asize + (16 - asize%16);


    // check to see if the size is minimum
    if(asize < 32)
        asize = 32;


    bp = sf_mem_start();

    if(bp == sf_mem_end()){
        bp = sf_mem_grow();
        if(bp == NULL){
            sf_errno = ENOMEM;
            return NULL;
        }


        //sets the prolog header for the heap
        sf_prologue  *plp = (sf_prologue *)(bp);
        plp->header.info.allocated = 1;
        plp->header.info.prev_allocated = 0;
        plp->header.info.block_size = 0;
        plp->header.info.requested_size = 0;

        // sets the prolog footer for the heap
        plp->footer.info.allocated = 1;
        plp->footer.info.prev_allocated = 0;
        plp->footer.info.block_size = 0;
        plp->footer.info.requested_size = 0;


        // sets the epilog of the heap
        sf_epilogue *elp = (sf_epilogue *)(sf_mem_end()-sizeof(sf_epilogue));
        elp->footer.info.allocated = 1;
        elp->footer.info.prev_allocated = 0;
        elp->footer.info.block_size = 0;
        elp->footer.info.requested_size = 0;

        // advances bp to point to the header of hte block
        bp = bp + sizeof(sf_prologue);

        // makes header pointer from bp's current address
        sf_header *hdp = (sf_header *)(bp);

        // calculates the blocksize for this free block
        size_t blocksz = PAGE_SZ - sizeof(sf_prologue) - sizeof(sf_epilogue);


        //sets the header of this free block
        hdp->info.allocated = 0;
        hdp->info.prev_allocated = 1;
        hdp->info.block_size = (blocksz>>4);
        hdp->info.requested_size = 0;

        // sets the footer of this free block
        sf_footer *ftp = (sf_footer *)((sf_mem_end() - sizeof(sf_epilogue) - sizeof(sf_footer)));
        ftp->info.allocated = 0;
        ftp->info.prev_allocated = 1;
        ftp->info.block_size = (blocksz>>4);
        ftp->info.requested_size = 0;

        //creates new size group in the freelist and sets the block headers links
        sf_add_free_list(blocksz, &sf_free_list_head);
        insert_to_list(&sf_free_list_head, hdp);


    }


    // // search the free list for a best fit and place it

    while( (bp = find_bestfit(asize)) == NULL){

        if((bp = sf_mem_grow()) == NULL){
            sf_errno = ENOMEM;
            return NULL;
        }
        //block header
        sf_header  *hdp = (sf_header *)(bp - sizeof(sf_epilogue));
        hdp->info.requested_size = 0;
        hdp->info.block_size = (PAGE_SZ)>>4;
        hdp->info.allocated = 0;

        //block footer
        sf_footer *bft =  (sf_footer *)(sf_mem_end()- sizeof(sf_epilogue) - sizeof(sf_footer));
        bft->info.requested_size = 0;
        bft->info.block_size = (PAGE_SZ)>>4;
        bft->info.allocated = 0;
        bft->info.prev_allocated = hdp->info.prev_allocated;

        sf_epilogue *eptr = (sf_epilogue *)(sf_mem_end()- sizeof(sf_epilogue));
        eptr->footer.info.allocated = 1;
        eptr->footer.info.prev_allocated = 0;
        insert_to_list(&sf_free_list_head, hdp);
        bp = coalesce( (void*)(hdp));

    }

    // placeit will get the block pointer and allocate the block, if split is required, it will split the block, and put the free block into the list!
    placeIt( (void *)(bp), asize, size);

    return (bp+ sizeof(sf_block_info));

}


/**
this function takes the fixed size from the malloc and search the list exhaustively for a bestfit block
@return it returns the best fit  block's blockheader if  found, else it  returns null
**/


void *find_bestfit(size_t size){

    sf_free_list_node *ptr = &sf_free_list_head;


    // if the list is empty

    if( ptr->next == &sf_free_list_head && ptr->prev == &sf_free_list_head)
        return NULL;


    // pointer to keep track of the address of the minimum fit
    sf_header  *temp;

    // traverse the list to find the best fit
    for(ptr = ptr->next; ptr != &sf_free_list_head; ptr = ptr->next){

        if(ptr->size >= size){
            temp = &(ptr->head);

            if(temp->links.next == &ptr->head && temp->links.prev == &ptr->head){
                continue;
            }else{

                sf_header *hp = temp->links.next;
                remove_from_blocklist(hp);
                return hp;
            }

        }
    }

    return NULL;

}


void placeIt(void* bp, size_t asize, size_t size){



    sf_header *ptr = (sf_header *)(bp);

    size_t csize = (ptr->info.block_size)<<4;
    if((csize-asize) >32 && (csize-asize)%16 == 0 ){


        ptr->info.requested_size = size;
        ptr->info.block_size = (asize)>>4;
        ptr->info.two_zeroes = 0;
        ptr->info.allocated = 1;
        // not changing  prev_allocated

        // going into the next block
        sf_header *temp = (sf_header *)( (void*)(ptr)  + asize);
        temp->info.requested_size = 0;
        size_t bsz = (csize- asize);
        temp->info.block_size = bsz>>4;
        temp->info.two_zeroes = 0;
        temp->info.allocated = 0;
        temp->info.prev_allocated = 1;

        //sf_footer *fpt = (bp+csize - sizeof(sf_footer));
        sf_footer *fpt = (sf_footer *)((void *)(ptr) + csize - sizeof(sf_footer));
        fpt->info.requested_size = 0;
        fpt->info.block_size = bsz>>4;
        fpt->info.two_zeroes = 0;
        fpt->info.allocated = 0;
        fpt->info.prev_allocated = 1;

        sf_header *next_head = (sf_header *)((void*)(fpt) + sizeof(sf_footer));
        next_head->info.prev_allocated = 0;
        if(!next_head->info.allocated){
            sf_footer *nft = (sf_footer *)(  (void *)(next_head) + ((next_head->info.block_size)<<4)    );
            nft->info.prev_allocated = 0;
        }



        insert_to_list(&sf_free_list_head, temp);

    }else{

        ptr->info.requested_size = size;
        ptr->info.allocated = 1;
        ptr->info.two_zeroes = 0;

        sf_header *nextblock = (sf_header *)((void *)(ptr) + ((ptr->info.block_size)<<4));
        nextblock->info.prev_allocated = 1;
        if(!nextblock->info.allocated){

            sf_footer * nextftr = (sf_footer *)( (void *)(nextblock) + ((nextblock->info.block_size)<<4) - sizeof(sf_footer));
            nextftr->info.prev_allocated = 1;
        }


    }

}

void *coalesce(void *bp){


    sf_header *curr = (sf_header *)(bp);
    sf_block_info *current_inf = (sf_block_info *)((void*)(&curr->info));
    sf_block_info *prev_inf = (sf_block_info *)(  (void*)(curr) - sizeof(sf_footer));
    sf_block_info *next_inf = (sf_block_info *)((void*)(curr)  + ((curr->info.block_size)<<4));


    if(current_inf->prev_allocated && next_inf->allocated){
        return bp;

    }else if( current_inf->prev_allocated && !next_inf->allocated){

        //merge with the next block
        sf_header *nextblock = (sf_header *)( bp + ((current_inf->block_size)<<4));
        size_t newsize = ((current_inf->block_size)<<4) + ((next_inf->block_size)<<4);
        remove_from_blocklist(curr);
        remove_from_blocklist(nextblock);
        curr->info.block_size = (newsize>>4);
        sf_footer *blockfooter =  (sf_footer *)((void *)(nextblock) + ((next_inf->block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        blockfooter->info.prev_allocated = curr->info.prev_allocated; //newly added
        insert_to_list(&sf_free_list_head, curr);


    }else if(!current_inf->prev_allocated && next_inf->allocated){

        //merge with the previous block
        sf_header *prevblock = (sf_header *)(bp - ((prev_inf->block_size)<<4)  );
        size_t newsize = ((prev_inf->block_size)<<4) + ((curr->info.block_size)<<4);
        remove_from_blocklist(prevblock);
        remove_from_blocklist(curr);
        prevblock->info.block_size = (newsize>>4);
        sf_footer *blockfooter  = (sf_footer *)((void *)(curr) + ((curr->info.block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        blockfooter->info.prev_allocated = prevblock->info.prev_allocated; //newly added
        curr = prevblock;
        insert_to_list(&sf_free_list_head, curr);

    }else{

        sf_header *prevblock = (sf_header *)(bp - ((prev_inf->block_size)<<4));
        sf_header *nextblock = (sf_header *)( bp + ((current_inf->block_size)<<4));

        size_t newsize = ((prev_inf->block_size)<<4) + ((curr->info.block_size)<<4) + ((next_inf->block_size)<<4);
        remove_from_blocklist(prevblock);
        remove_from_blocklist(curr);
        remove_from_blocklist(nextblock);

        prevblock->info.block_size = newsize>>4;
        sf_footer *blockfooter =  (sf_footer *)((void *)(nextblock) + ((next_inf->block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        blockfooter->info.prev_allocated = prevblock->info.prev_allocated; //newly added
        curr = prevblock;
        insert_to_list(&sf_free_list_head, curr);
    }



    return (bp=(void*)(curr));
}


void remove_from_blocklist(sf_header *blockptr){

    blockptr->links.prev->links.next = blockptr->links.next;
    blockptr->links.next->links.prev = blockptr->links.prev;
}

void pushblock(sf_header *head, sf_header *node){


    node->links.next = head->links.next;
    node->links.next->links.prev = node;
    node->links.prev = head;
    head->links.next = node;

}


/*
takes parameterd : head of the free list and block to be inserted
if list is empty -> creat class block and insert the block
if list has class size -> put it in
if list does not have a class size->
    find proper position and create new class size
    put the block
*/

void insert_to_list(sf_free_list_node *node, sf_header *block){

    if(node->next == &sf_free_list_head && node->prev == &sf_free_list_head){

        sf_free_list_node *ptr = sf_add_free_list( ((block->info.block_size)>>4), node);
        pushblock(&ptr->head, block);
        return;
    }

    for(node= node->next; node != &sf_free_list_head; node= node->next){

        if(node->size ==((block->info.block_size)<<4)){
            pushblock(&node->head, block);
            return;
        }

    }


    node = (sf_free_list_node *)(&sf_free_list_head);

    for(node = node->next; node != &sf_free_list_head; node = node->next){

        if(node->size > ((block->info.block_size)<<4)){
            break;
        }
    }


    sf_free_list_node *ptr = sf_add_free_list( ((block->info.block_size)<<4), node);
    pushblock(&ptr->head, block);


}




void sf_free(void *pp) {

    // check if pointer is null
    if(pp == NULL)
        abort();

    sf_header  *ptr = (sf_header *)(pp - sizeof(sf_block_info));

    //header of the block is bfr end of prolog or after beginning epilog
    if( (void *)ptr < (sf_mem_start()+sizeof(sf_prologue)))
        abort();

    if((void *)ptr >= (sf_mem_end() - sizeof(sf_epilogue)))
        abort();


    //allocated bit in the header or footer is zero
    if(ptr->info.allocated == 0)
        abort();

    //blocksize ins not mutiple of 16 or less than minimum=32
    if( (((ptr->info.block_size)<<4)%16 != 0 ) ||  (((ptr->info.block_size)<<4) <32))
        abort();

    // requested_sz + sizeof(blockheader) > block_size
    if(   (ptr->info.requested_size+ sizeof(sf_block_info)) >  ((ptr->info.block_size)<<4))
        abort();


    //if prev_alloc ==0 check alloc fields of header and footer
    if(ptr->info.prev_allocated == 0){

        sf_footer *prev_footer = (sf_footer *)( (void *)(ptr) - sizeof(sf_footer));
        sf_header *prev = (sf_header *)( (void *)(ptr) - ( (prev_footer->info.block_size)<<4));

        if(prev->info.allocated != 0  || prev_footer->info.allocated != 0)
            abort();
    }


    size_t blocksize  =  (ptr->info.block_size)<<4;

    sf_footer *ftr =  (sf_footer *)((void *)(ptr) + blocksize - sizeof(sf_footer));

    ptr->info.block_size = blocksize>>4;
    ptr->info.requested_size = 0;
    ptr->info.allocated = 0;
    ptr->info.two_zeroes = 0;


    ftr->info.block_size = blocksize>>4;
    ftr->info.requested_size = 0;
    ftr->info.allocated = 0;
    ftr->info.prev_allocated = ptr->info.prev_allocated;
    ftr->info.two_zeroes = 0;

    // // modify the next block prev alloc bit
     sf_header  *nextblock = (sf_header *)( (void *)(ftr) + sizeof(sf_footer));
    nextblock->info.prev_allocated = 0;
     if(!nextblock->info.allocated){
         size_t nbs = nextblock->info.block_size<<4;
         sf_footer *nf= (sf_footer *)( (void *)(nextblock) + nbs - sizeof(sf_footer));
         nf->info.prev_allocated = 0;
     }

    insert_to_list(&sf_free_list_head, ptr);
    ptr = coalesce((void*)(ptr));

    return;
}




void *sf_realloc(void *pp, size_t rsize) {

   // return  NULL;

    if(rsize <= 0){
        sf_free(pp);
        return NULL;
    }

    // check if pointer is null
    if(pp == NULL)
        abort();

    sf_header  *ptr = (sf_header *)(pp - sizeof(sf_block_info));

    //header of the block is bfr end of prolog or after beginning epilog
    if( (void *)ptr < (sf_mem_start()+sizeof(sf_prologue)))
        abort();

    if((void *)ptr >= (sf_mem_end() - sizeof(sf_epilogue)))
        abort();


    //allocated bit in the header or footer is zero
    if(ptr->info.allocated == 0)
        abort();

    //blocksize ins not mutiple of 16 or less than minimum=32
    if( (((ptr->info.block_size)<<4)%16 != 0 ) ||  (((ptr->info.block_size)<<4) <32))
        abort();

    // requested_sz + sizeof(blockheader) > block_size
    if(   (ptr->info.requested_size+ sizeof(sf_block_info)) >  ((ptr->info.block_size)<<4))
        abort();


    //if prev_alloc ==0 check alloc fields of header and footer
    if(ptr->info.prev_allocated == 0){

        sf_footer *prev_footer = (sf_footer *)( (void *)(ptr) - sizeof(sf_footer));
        sf_header *prev = (sf_header *)( (void *)(ptr) - ( (prev_footer->info.block_size)<<4));

        if(prev->info.allocated != 0  || prev_footer->info.allocated != 0)
            abort();
    }

    if(rsize == ptr->info.requested_size){

        return pp;
    }else if(rsize > ptr->info.requested_size){
        //call malloc and copy memory
        void *newptr = sf_malloc(rsize);
        if(newptr == NULL)
            return NULL;
        //size_t sz =  ((ptr->info.block_size)<<4) - sizeof(sf_block_info);
        memcpy((void*)(newptr), (const void *)(pp), ptr->info.requested_size);

        //free takes care of the bit no worry
        sf_free(pp);
        return newptr;
    }else{

        size_t sz = rsize + sizeof(sf_block_info);
        if(sz%16 != 0){
            sz = sz+ (16 - sz%16);
        }

        if(sz<32){
            sz= 32;
            //return pp;
        }

        if( (((ptr->info.block_size)<<4) -sz)<32   ){
            ptr->info.requested_size = rsize;
           sf_footer *footer = (sf_footer *)( (void *)(ptr) + ((ptr->info.block_size)<<4) - sizeof(sf_footer));
           footer->info.requested_size = rsize;
            return pp;
        }

        if(  ((((ptr->info.block_size)<<4) -sz) >= 32)){

            size_t alc = sz;
            size_t fr = (((ptr->info.block_size)<<4) -sz);

            sf_header *freeblock = (sf_header *)( (void *)(ptr) + alc);
            freeblock->info.requested_size = 0;
            freeblock->info.block_size = fr>>4;
            freeblock->info.two_zeroes = 0;
            freeblock->info.prev_allocated =1;
            freeblock->info.allocated = 0;

            ptr->info.requested_size = rsize;
            ptr->info.block_size = alc>>4;
            // ptr->info.two_zeroes = 0;
            // ptr->info.allocated = 1;

            sf_footer *freefooter = (sf_footer *)((void *)(freeblock) +  ((freeblock->info.block_size)<<4) - sizeof(sf_footer));
            freefooter->info.requested_size = 0;
            freefooter->info.block_size = fr>>4;
            freefooter->info.two_zeroes = 0;
            freefooter->info.prev_allocated = 1;
            freefooter->info. allocated = 0;

             //sf_header  *nextblock = (sf_header *)( (void *)(freefooter) + sizeof(sf_footer));
             //   nextblock->info.prev_allocated = 0;
            // // modify the next block prev alloc bit
            sf_header  *nextblock = (sf_header *)( (void *)(freefooter) + sizeof(sf_footer));
            nextblock->info.prev_allocated = 0;
            if(!nextblock->info.allocated){
                size_t nbs = nextblock->info.block_size<<4;
                sf_footer *nf= (sf_footer *)( (void *)(nextblock) + nbs - sizeof(sf_footer));
                nf->info.prev_allocated = 0;
            }


            insert_to_list(&sf_free_list_head, freeblock);
            coalesce(freeblock);

            return  pp;
        }//else{

        //    // ptr->info.requested_size = 32;
        //    // sf_footer *footer = (sf_footer *)( (void *)(ptr) + ((ptr->info.block_size)<<4) - sizeof(sf_footer));
        //    // footer->info.requested_size = 32;
        //     return pp;
        // }

    }





    return NULL;
}
