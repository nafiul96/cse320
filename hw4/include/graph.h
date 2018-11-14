


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



graph *g;

graph *creategraph();

int addgnode(graph *g,gnode *node);
gnode *findbytype(char *tp);

int addgedge(gnode *to, gnode *from);

void populatenode(gnode *gn, char *t);

int linkme(char *from, char *to);






void printgraph(graph *g);


