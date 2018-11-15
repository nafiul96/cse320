


typedef struct node{
    char *type;
}node;

typedef struct conv{
    char *prog;
    char **args;
}conv;

typedef struct adjNode{

    node *n;
    conv *conversion;
    struct adjNode *next;

}adjNode;



typedef struct adjlist{
    adjNode * head;

}adjlist;



typedef struct gnode{
    node *n;
    int degree;
    int seen;
    adjlist * list;
    struct gnode *next;

}gnode;



typedef struct graph{

    int v;
    gnode *head;

}graph;




typedef struct pathnode{

    node *n;
    struct pathnode *next;
    struct pathnode *prev;
}pathnode;



typedef struct pathQ
{
    pathnode *front;
    pathnode *rear;
    int count;
}pathQ;






graph *g;
pathnode *path;



graph *creategraph();

int addgnode(graph *g,gnode *node);
gnode *findbytype(char *tp);

adjNode *addgedge(gnode *to, gnode *from);

void populatenode(gnode *gn, char *t);

int linkme(char *from, char *to, conv *conversion);

pathnode *findpath(graph *g, char *start, char *end);

int iseeyou(pathnode *head, adjNode *node);

adjNode *findadjbytype(adjNode *node ,char *type);

void printpath(pathnode *p);

void printgraph(graph *g);


