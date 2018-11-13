
/**
 * Graph datastructure implemented using adjacency list
 */







/**
 * Structures to hold the graph configurations:
 * graphnode
 * edge
 * edgelist
 */


/**
 * Utility Macros
 */

#define VISITED 1
#define NOT_VISITED 0


typedef struct edgelist;


/**
 * The node in the graph for the specific type
 * contains type, edgelist, and flag to check visited or not
 */
typedef struct graphnode{

char *type;
struct edgelist *list;
int visited;

}graphnode;


/**
 * represents edge of the graph node
 * contains node to which current node to be connected, a conversion program, and arguments for the program
 */
typedef struct edge{

    graphnode *gnode;
    char *program;
    char**args;

}edge;



/**
 * doubly circular linkedlist as edgelist for a graphnode
 */
typedef struct edgelist{

    edge *head;
    edge *next;
    edge *prev;


}edgelist;


typedef struct graph{

    graphnode *head;
    graphnode *next;

}graph;



/**
 * Utility variables
 */
int pathlength;
int node_count;
edgelist *bfs;



/**
 * The methods for the graph to be implemented
 */

void addNode(graphnode *node);


void addEdge(graphnode *from, graphnode *to);

int pathexists(graphnode *from, *graphnode *to);

edgelist *findpath(graphnode *start, graphnode *end);


graphnode *findNode(char *type);






