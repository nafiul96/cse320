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


    asize  =  size + sizeof(sf_header);


    //check to see if the size is a multiple of 16
    if(asize%16 != 0)
        asize  = asize + (16 - asize%16);


    // check to see if the size is minimum
    if(asize < 32) return NULL;


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
        sf_free_list_node *ptr = sf_add_free_list(blocksz, &sf_free_list_head);
        hdp->links.next  = &ptr->head;
        hdp->links.next->links.prev = &ptr->head;
        hdp->links.next->links.prev= hdp;
        ptr->head.links.next = hdp;

    }


    // // search the free list for a best fit and place it
    // if(  (bp = find_bestfit(asize)) != NULL){

    //     placeIt( (void *)(bp), asize, size);
    //     return (void*)bp+sizeof(sf_header);
    // }
    while( (bp = find_bestfit(asize)) == NULL){

        if((bp = sf_mem_grow()) == NULL){
            sf_errno = ENOMEM;
            return NULL;
        }
        //bp = bp - sizeof(sf_footer);
        sf_header  *hdp = (sf_header *)(bp - sizeof(sf_epilogue));
        hdp->info.block_size = (PAGE_SZ)>>4;
        hdp->info.allocated = 0;

        sf_epilogue *eptr = (sf_epilogue *)(sf_mem_end()- sizeof(sf_epilogue));
        eptr->footer.info.allocated = 1;
        eptr->footer.info.prev_allocated = 0;
        insert_to_list(&sf_free_list_head, hdp);
        bp = coalesce( (void*)(hdp));

    }

    // placeit will get the block pointer and allocate the block, if split is required, it will split the block, and put the free block into the list!
    placeIt( (void *)(bp), asize, size);

    return (bp+ sizeof(sf_block_info));





    //return NULL;
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

    //pointer to traverse the list of lists
    //ptr = &sf_free_list_head;

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
                temp->links.next = hp->links.next;
                hp->links.next->links.prev = hp->links.prev;
                // temp->links.prev->links.next = temp->links.next;
                // temp->links.next->links.prev = temp->links.prev;
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
        sf_header *temp = (bp+ asize);
        temp->info.requested_size = 0;
        temp->info.block_size = (csize - asize)>>4;
        temp->info.two_zeroes = 0;
        temp->info.allocated = 0;
        temp->info.prev_allocated = 1;

        //sf_footer *fpt = (bp+csize - sizeof(sf_footer));
        sf_footer *fpt = (bp+csize - sizeof(sf_footer));
        fpt->info.requested_size = 0;
        fpt->info.block_size = (csize-asize)>>4;
        fpt->info.two_zeroes = 0;
        fpt->info.allocated = 0;
        fpt->info.prev_allocated = 1;

        sf_footer *next_head = (sf_footer *)((void*)(fpt) + sizeof(sf_footer));
        next_head->info.prev_allocated = 0;

        //need to split and insert
        sf_free_list_node *nptr = (sf_free_list_node *)(&sf_free_list_head);

        for(nptr = nptr->next; nptr != &sf_free_list_head; nptr = nptr->next){
            if(nptr->size ==(csize-asize)){

                ptr->links.prev = &nptr->head;
                ptr->links.next = nptr->head.links.next;
                nptr->head.links.next->links.next = ptr;
                nptr->head.links.next = ptr;
                return ;
            }
        }

        for(nptr = nptr->next; nptr != &sf_free_list_head; nptr= nptr->next){

            if(nptr->size >=(csize-asize)){
            break;
            }
        }



        sf_free_list_node *node  = sf_add_free_list((csize-asize), nptr);

        ptr->links.prev = node->head.links.next;
        ptr->links.next = node->head.links.prev;
        node->head.links.next->links.prev = ptr;
        node->head.links.next = ptr;


    }else{

        ptr->info.requested_size = size;
        ptr->info.allocated = 1;
        ptr->info.two_zeroes = 0;

    }

}

void *coalesce(void *bp){

    sf_header *curr = (sf_header *)(bp);
    size_t blocksize = (curr->info.block_size)<<4;
    sf_header *nextblock  = (sf_header *)(bp+ blocksize);
    size_t prev_alloc = curr->info.prev_allocated;
    size_t next_alloc = nextblock->info.allocated;


    if(prev_alloc && next_alloc){
        return bp;

    }else if( prev_alloc && !next_alloc){

        size_t newsize = ((curr->info.block_size)<<4) + ((nextblock->info.block_size)<<4);
        remove_from_blocklist(curr);
        remove_from_blocklist(nextblock);
        sf_footer *blockfooter  = (sf_footer *)((void *)(nextblock) + ((nextblock->info.block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        insert_to_list(&sf_free_list_head, curr);


    }else if(!prev_alloc && next_alloc){

        //merge with the previous block
        sf_block_info *previnfo = (sf_block_info *)(bp - sizeof(sf_footer));
        blocksize =previnfo->block_size;
        sf_header *prevblock = (sf_header *)(bp - blocksize);
        size_t newsize = ((previnfo->block_size)<<4) + ((curr->info.block_size)<<4);
        remove_from_blocklist(prevblock);
        remove_from_blocklist(curr);
        prevblock->info.block_size = (newsize>>4);
        sf_footer *blockfooter  = (sf_footer *)((void *)(curr) + ((curr->info.block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        curr = prevblock;
        insert_to_list(&sf_free_list_head, curr);

    }else{

        //mege both of the blocks
        sf_block_info *previnfo = (sf_block_info *)(bp - sizeof(sf_footer));
        blocksize =previnfo->block_size;
        sf_header *prevblock = (sf_header *)(bp - blocksize);
        size_t newsize = ((previnfo->block_size)<<4) + ((curr->info.block_size)<<4) + ((nextblock->info.block_size)<<4);
        remove_from_blocklist(prevblock);
        remove_from_blocklist(curr);
        remove_from_blocklist(nextblock);
        prevblock->info.block_size = (newsize>>4);
        sf_footer *blockfooter  = (sf_footer *)((void *)(nextblock) + ((nextblock->info.block_size)<<4) - sizeof(sf_footer));
        blockfooter->info.block_size = newsize>>4;
        curr = prevblock;
        insert_to_list(&sf_free_list_head, curr);

    }



    return (bp=(void*)(curr));
}


void remove_from_blocklist(sf_header *blockptr){

    sf_header *curr = blockptr;
    sf_header *prev =  blockptr->links.prev;
    sf_header *next = blockptr->links.next;

    prev->links.next = curr->links.next;
    next->links.prev = prev;
}

void pushblock(sf_header *head, sf_header *node){

    sf_header *next = head->links.next->links.next;
    sf_header *prev = head;
    sf_header *curr= node;

    curr->links.next  = next;
    curr->links.prev = head;
    prev->links.next = node;
    next->links.prev = node;


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


    node = &sf_free_list_head;

    for(node = node->next; node != &sf_free_list_head; node = node->next){

        if(node->size >= ((block->info.block_size)<<4)){
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
    ftr->info.two_zeroes = 0;

    insert_to_list(&sf_free_list_head, ptr);
    ptr = coalesce((void*)(ptr));

    return;
}

void *sf_realloc(void *pp, size_t rsize) {
    return NULL;
}
