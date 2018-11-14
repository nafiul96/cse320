
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



int addgedge(gnode *from, gnode *to){

    // get head of the adjlist of to node
    adjNode *temp = from->list->head;

    // check to see if any edge there
    if(temp==NULL){

        //malloc for edge
        adjNode *edge = malloc( sizeof(adjNode));

        if(edge == NULL)
            return 0;

        edge->n = from->n;
        char *pr = malloc(25);
        if(pr == NULL)
            return 0;

        strcpy(pr, "bin/cat");
        conv *con = malloc( sizeof(conv));
        if(con == NULL)
            return 0;

        con->prog = pr;
        con->args = NULL;
        edge->conversion = con;
        edge->next = NULL;
        temp = edge;
        from->degree = from->degree +1;
        return 1;
    }


    //malloc for edge
        adjNode *edge = malloc( sizeof(adjNode));

        if(edge == NULL)
            return 0;

        edge->n = from->n;
        char *pr = malloc(25);
        if(pr == NULL)
            return 0;

        strcpy(pr, "bin/cat");
        conv *con = malloc( sizeof(conv));
        if(con == NULL)
            return 0;

        con->prog = pr;
        con->args = NULL;
        edge->conversion = con;
        edge->next = temp;
        from->list->head = edge;
        from->degree = from->degree +1;
        return 1;


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

int linkme(char *from, char *to){


    gnode *f = NULL;
    gnode *t= NULL;

    if(   (f=findbytype(from)) == NULL ){
        return 0;
    }

    if( (f=findbytype(from)) == NULL   ){
        return 0;
    }

    return addgedge(f, t);


}













void printgraph(graph *g){

    gnode *temp = NULL;

    for(temp = g->head; temp != NULL; temp = temp->next){
        printf("type is: %s\n", temp->n->type);
    }


}













