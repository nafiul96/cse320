
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "graph.h"


graph *creategraph(){

    graph *g = malloc( sizeof(graph) );

    if(g == NULL){
        fprintf(stderr, "memory error\n");
    }

    g->v= 0;
    g->head = NULL;
    return g;

}


int addgnode(graph *g,gnode *node){

    if(g->head == NULL || g->v == 0){
        node->next = NULL;
        g->head = node;
        g->v = g->v +1;
        return 1;
    }

    //append to head
    node->next = g->head;
    g->head = node;
    g->v = g->v +1;
    return 1;



}


gnode *findbytype(char *tp){


    gnode *temp = NULL;

    for(temp = g->head; temp != NULL; temp = temp->next){
        if( strcmp(temp->n->type,tp) ==0 ){
            return temp;
        }
    }

    return NULL;
}



adjNode *addgedge(gnode *from, gnode *to){

    // get head of the adjlist of to node
    adjNode *temp = from->list->head;

    // check to see if any edge there
    if(temp==NULL){

        //malloc for edge
        adjNode *edge = malloc( sizeof(adjNode));

        if(edge == NULL)
            return NULL;

        edge->n = to->n;
        char *pr = malloc(25);
        if(pr == NULL)
            return NULL;

        strcpy(pr, "bin/cat");
        conv *con = malloc( sizeof(conv));
        if(con == NULL)
            return NULL;

        con->prog = pr;
        con->args = NULL;
        edge->conversion = con;
        edge->next = NULL;
        //temp = edge;
        from->list->head = edge;
        from->degree = from->degree +1;
        return edge;
    }


    //malloc for edge
        adjNode *edge = malloc( sizeof(adjNode));

        if(edge == NULL)
            return NULL;

        edge->n = to->n;
        char *pr = malloc(25);
        if(pr == NULL)
            return NULL;

        strcpy(pr, "bin/cat");
        conv *con = malloc( sizeof(conv));
        if(con == NULL)
            return NULL;

        con->prog = pr;
        con->args = NULL;
        edge->conversion = con;
        edge->next = temp;
        from->list->head = edge;
        from->degree = from->degree +1;
        return edge;


}



void populatenode(gnode *gn, char *t){

    char *tp = malloc(strlen(t));
    strcpy(tp,t);

    // for the type-node
    node *nd = malloc( sizeof(node)  );
    nd->type = tp;

    // for the adjacency list
    adjlist *tmplist = malloc( sizeof(adjlist) );
    tmplist->head = NULL;

    // for the graph node
    gnode *temp = gn;
    temp->n = nd;
    temp->degree = 0;
    temp->seen = 0;
    temp->list = tmplist;




}

int linkme(char *from, char *to, conv *conversion){


    gnode *f = NULL;
    gnode *t= NULL;

    if(   (f=findbytype(from)) == NULL ){
        return 0;
    }

    if( (t=findbytype(to)) == NULL   ){
        return 0;
    }

    adjNode *tmp = addgedge(f, t);
    if(tmp == NULL)
        return 0;
    tmp->conversion = conversion;
    //return (addgedge(f, t) != NULL);
    return 1;


}



int iseeyou(pathnode *head, adjNode *node){

    pathnode *temp;
    for(temp = head; temp != NULL; temp = temp->next){

        if(   strcmp(temp->n->type,node->n->type) == 0){
            return 1;
        }


    }

    return 0;
}



pathnode *findpath(graph *g, char *start, char *end){


    // get the graph nodes
    gnode *s = findbytype(start);
    gnode *e = findbytype(end);


    //this is the queue
    pathQ *q = (pathQ *)malloc( sizeof(pathQ) );
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;

    // create pathnode with the starting node
    pathnode *temp = malloc( sizeof(pathnode) );
    temp->n = s->n;
    temp->next = NULL;
    temp->prev = NULL;


    //actual path to be returned
    pathnode *head = NULL;
    pathnode *tail = NULL;

    // insert into the queue
    q->front = temp;
    q->rear = temp;
    q->count = q->count + 1;


    while(q->count >0){


        gnode *inter = findbytype(q->front->n->type);
        adjNode *craps = NULL;

        for(craps = inter->list->head; craps != NULL; craps = craps->next){

            if( iseeyou(q->front,craps) || iseeyou(head,craps) ){
                continue;

            }else{


                // create pathnode with the crap node
                pathnode *crap = malloc( sizeof(pathnode) );
                crap->n = craps->n;
                crap->next = NULL;
                crap->prev = NULL;

                //enqueue crap
                crap->prev = q->rear;
                q->rear->next = crap;
                q->rear = crap;
                q->count = q->count + 1;


            }


        }

        //it's time to add to the path by dequeuing
        if(head == NULL){

            // add to the path
            head = q->front;
            tail = head;

            q->front = q->front->next;

            head->prev = NULL;
            tail->next = NULL;

            q->count = q->count -1;


        }else{

            //add to the tail of the path by dequeuing
            pathnode *dq = q->front;
            q->front = q->front->next;


            dq->prev = tail;
            tail->next = dq;
            tail = dq;
            tail->next = NULL;
            q->count = q->count -1;



        }
        if( strcmp(tail->n->type,e->n->type) == 0){


            pathnode *tmp = tail;

            if(tmp == head){
                return head;
            }else if(tmp->prev == head){
                return head;
            }
            tmp = tmp->prev;

            while(tmp != head){

                gnode *n = findbytype(tmp->n->type);
                if( findadjbytype(n->list->head, tmp->next->n->type) == NULL ){

                    tmp = tmp->prev;
                    tmp->next->next->prev = tmp;
                    tmp->next = tmp->next->next;

                }else{
                    tmp = tmp->prev;
                }
            }


            return head;
        }




    }


    return NULL;

}







void printpath(pathnode *p){
    if( p == NULL )
        return;

    pathnode *temp;
    for(temp = p; temp != NULL; temp = temp->next){
        printf("%s->", temp->n->type);
    }
    printf("\n");
}



adjNode *findadjbytype(adjNode *node ,char *type){

    adjNode *temp;
    for(temp = node; temp != NULL; temp = temp->next){

        if( strcmp(temp->n->type, type) == 0){
            return temp;
        }

    }

    return NULL;

}





void printgraph(graph *g){

    gnode *temp = NULL;

    for(temp = g->head; temp != NULL; temp = temp->next){
        printf("type is: %s\n", temp->n->type);
    }


}













