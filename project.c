#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t=0;
int tt = 0;
int **mat;
#define MAX_VERTICES 100// Maximum number of vertices
#define MAX_EDGES 1000// Maximum number of edges

typedef struct Stack stack;

struct Stack { 
    int top; 
    int capacity; 
    int* array; 
}; 
  
// function to create a stack of given capacity. It initializes size of 
// stack as 0 
struct Stack* createStack(int capacity) 
{ 
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack)); 
    stack->capacity = capacity; 
    stack->top = -1; 
    stack->array = (int*)malloc(stack->capacity * sizeof(int)); 
    return stack; 
} 
  
// Stack is full when top is equal to the last index 
int isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
} 
  
// Stack is empty when top is equal to -1 
int isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1; 
} 

void push(struct Stack* stack, int item) 
{ 
    if (isFull(stack)) 
        return; 
    stack->array[++stack->top] = item; 
   // printf("%d pushed to stack\n", item); 
} 
  
// Function to remove an item from stack.  It decreases top by 1 
int pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return -1; 
    return stack->array[stack->top--]; 
} 

int top(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return -1; 
    return stack->array[stack->top]; 
} 


typedef struct node node;

struct node
{
    int data;
    struct node *next;
};

typedef struct graph graph;

struct graph
{   
    int vertices;
    int edges;
    node **adjlist;
    int *dis;
    int *finish;
    int *component;
    int comp;
};

int min_two(int n , int m )
{
    if( n<m )
       return n;
    else
      return m;   

}



void insert_link(node **root,int key)           //structure for inserting nodes in adjacency list
{
    node *current, *prev;
    current=(node*)malloc(sizeof(node));
    current->data=key;
    current->next=NULL;
    if(*root==NULL)
    *root=current;
    else
    {   prev=*root;
        while(prev->next!=NULL)
            prev=prev->next;
        prev->next=current;
    }
}

void rand_graph(graph *g)
{
    int numVertices, numEdges, i, j;
    
    srand(time(NULL));

    // Define the number of vertices and edges
    numVertices = rand() % MAX_VERTICES + 1;
    numEdges = rand() % MAX_EDGES + 1;
    mat=(int**)malloc(numVertices*sizeof(int*));
    for(int i=0;i<numVertices;i++)
    mat[i]=(int*)malloc(numVertices*sizeof(int));

    for (i = 0; i < numVertices; i++) {
        for (j = 0; j < numVertices; j++) {
            mat[i][j] = 0;
        }
    }

    if (numVertices < 2) {
        numVertices = 2; // Ensure at least 2 vertices for a graph
    }

    if (numEdges > (numVertices * (numVertices - 1)) / 2) {
        numEdges = (numVertices * (numVertices - 1)) / 2; // Ensure a simple graph
    }

    g->edges=numEdges;
    g->vertices=numVertices;
    g->adjlist=(node**)malloc(numVertices*sizeof(node*));
    g->comp = 1;

    g->component = (int *)malloc(g->vertices*sizeof(int));
    g->dis =  (int *)malloc(g->vertices*sizeof(int));
    g->finish =  (int *)malloc(g->vertices*sizeof(int));

    for(int k=0;k<numVertices;k++)
    g->adjlist[k]=NULL;

    // Generate random edges
    for (i = 0; i < numEdges; i++) {
        int source = rand() % numVertices;
        int destination = rand() % numVertices;

        // Ensure no self-loops 
        while (source == destination || mat[destination][source]==1 || mat[source][destination]==1){
            source = rand() % numVertices;
            destination = rand() % numVertices;
        }

        mat[source][destination] = 1;
        insert_link(&g->adjlist[source],destination);
        
    }
}

void top_dfs(graph *g,int arr[],int u,struct Stack *s)         //function for dfs
{   
        arr[u] = 1;
        t++;
        g->dis[u]=t;
        node *copy=g->adjlist[u];
        while(copy!=NULL)
        {   
            if(arr[copy->data]==0)
            {
                top_dfs(g,arr,copy->data,s);
            }
            
            copy=copy->next;
        }
                    
        t++;
        g->finish[u]=t;
        push(s,u);
        arr[u] = 2;

}


void topological_sort( graph *g , struct Stack *s )
{
    int arr[g->vertices];
    for(int i=0 ; i<g->vertices ; i++)
    {
        arr[i]=0;
    }

    for(int i=0 ; i<g->vertices ; i++)
    {
        if(arr[i] == 0)
        {
            top_dfs(g,arr,i,s);
        }
    }
}


void dfs(graph *g,int arr[],int u,struct Stack *s)         //function for dfs
{   
    //int time=0;
        arr[u] = 1;
        node *copy=g->adjlist[u];
        //int prev;
        while(copy!=NULL)
        {   
            if(arr[copy->data]==0)
            {
                g->component[copy->data] = g->component[u];
                dfs(g,arr,copy->data,s);
            }
            
            copy=copy->next;
        }
                    
        //printf("%d %d %s\n",ptr->x,ptr->y,ptr->type);
        arr[u] = 2;

}





void dfs_traversal( struct graph *g , struct Stack *s )
{
    g->comp = 1;
    int vis[g->vertices];
    for(int i=0 ; i<g->vertices ; i++)
    {
        vis[i]=0;
    }
    
    while( !isEmpty(s) )
    {
        int u = pop(s); 
        if(vis[u] == 0)
        { 
            g->component[u] = g->comp;
            dfs(g,vis,u,s);
            g->comp++;
        }

    }

}



/*void rev_graph(graph *g)
{   int arr[g->vertices][g->vertices];
    
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
          arr[i][j]=mat[j][i];
        }
    }
    
    for(int i=0;i<g->vertices;i++)
    g->adjlist[i]=NULL;
    for(int i=0;i<g->vertices;i++)
    {
        for(int j=0;j<g->vertices;j++)
        {
            if(arr[i][j]==1)
            insert_link(&g->adjlist[i],j);
        }
    }
}*/
void *rev_graph(graph *g)
{   
    graph *yes;
    yes=(graph*)malloc(sizeof(graph));
    yes->edges=g->edges;
    yes->vertices=g->vertices;
    yes->adjlist=(node**)malloc(g->vertices*sizeof(node*));
    yes->comp = 1;
    //printf("__");
    yes->component = (int *)malloc(g->vertices*sizeof(int));
    yes->dis =  (int *)malloc(g->vertices*sizeof(int));
    yes->finish =  (int *)malloc(g->vertices*sizeof(int));
    //printf("__");
    for(int i=0;i<g->vertices;i++)
    yes->adjlist[i]=NULL;
    for(int i=0;i<g->vertices;i++)
    {   //printf("__");
        node *copy=g->adjlist[i];
        //printf("__");
        while(copy!=NULL)
        {   //printf("__");
            insert_link(&yes->adjlist[i],copy->data);
            copy=copy->next;
        }
    }

    //display(yes);
    //g=NULL;
    //printf("%d",yes->adjlist[0]->data);
    for(int k=0;k<g->vertices;k++)
    g->adjlist[k]=NULL;

    for(int i=0;i<g->vertices;i++)
    {   //printf("__");
        node *copy=yes->adjlist[i];
        //printf("__");
        //printf("%d",copy->data);
        //node *copy=g->adjlist[i];
        g->adjlist[i]=NULL;
        while(copy!=NULL)
        {   //printf("__");
            insert_link(&g->adjlist[copy->data],i);
            //printf("%d",g->adjlist[copy->data]->data);
            //copy=copy->next;
            copy=copy->next;
        }
    }
    /*int arr[g->vertices][g->vertices];
    
    for (int i = 0; i < g->vertices; i++) {
        for (int j = 0; j < g->vertices; j++) {
          arr[i][j]=mat[j][i];
        }
    }
    
    for(int i=0;i<g->vertices;i++)
    g->adjlist[i]=NULL;
    for(int i=0;i<g->vertices;i++)
    {
        for(int j=0;j<g->vertices;j++)
        {
            if(arr[i][j]==1)
            insert_link(&g->adjlist[i],j);
        }
    }*/
}


void display( struct graph *g)           //function for printing the adjacency list
{
    for(int i=0;i<g->vertices;i++)
    {
        printf("Vertex %d: ",i);
        node *copy=g->adjlist[i];
        while(copy!=NULL)
        {
            printf("%d ",copy->data);
            copy=copy->next;
        }
        printf("\n");
    }
}



void kosaraju_algorithm(graph *g)
{

    struct Stack *s;
    s = createStack(g->vertices);

    topological_sort(g ,s);         // The first step in the kosaraju's algorithm is to sort the nodes of the graph according to their finishing time 
    rev_graph(g);                    // The second step in the kosaraju's algorithm is to reverse the graph
    dfs_traversal(g,s);                   // The third and the last step is to run dfs 

}


void tarjan_dfs ( graph *g , int u , int disc[] , int low[] , struct Stack *st , int in_stack[]  )
{
    disc[u] = low[u] = tt;
    tt++;
    in_stack[u] = 1;
    push(st,u);
    node *copy=g->adjlist[u];

        while(copy!=NULL)
        {  
            int v = copy->data; 
            if( disc[v] == -1 )
            {
                tarjan_dfs( g,v,disc,low,st,in_stack );
                low[u] = min_two( low[u] , low[v] );           
            }

            else if( in_stack[v] == 1 )
            {
                low[u] = min_two( low[u] , disc[v] );
            }
            copy=copy->next;
        }


        if(low[u]==disc[u]) 
       {
        while(top(st)!=u)
        {
            int m = pop(st);
            in_stack[m] = 0;
            g->component[m] = g->comp;
            
        }
        int m = pop(st);
        in_stack[m] = 0;
        g->component[m] = g->comp;
        g->comp++;

       }
  
}

void tarjan_algorithm( graph *g )               // This is the code implementing tarjan's algorithm
{

    int disc[g->vertices];                      
    int low[g->vertices];                       // we have to make an array that stores that value of the node with least discovery time connected to 
                                                // the given node
    int in_stack[g->vertices];
    g->comp = 1;
    struct Stack *st = createStack(g->vertices);  // we have to make an array that keep track if the node is in stack or not because it helps us to find
                                                  // the back edges
    for( int i=0 ; i<g->vertices; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        in_stack[i] = 0;
    }

    for( int i=0 ; i<g->vertices ; i++)
    {   
       if( disc[i]==-1 ) 
        {
            tarjan_dfs( g,i,disc,low,st,in_stack );  
        }    
    }

}


int main()
{   
    graph *g;
    g = (struct graph*)malloc(sizeof(struct graph));
    rand_graph(g);
    
    g->dis=(int*)malloc(g->vertices*sizeof(int));
    g->finish=(int*)malloc(g->vertices*sizeof(int));
   
    printf("\n");
    printf("Random Directed graph (Adjacency list)\n");
    display(g);
    
    char c1;
    printf("Enter 'T' for Tarjan's algorithm and 'K' for kosaraju's algorithm : ");
    scanf(" %c",&c1);
    
    switch( c1 )
    {
        case('T'):
                {
                   tarjan_algorithm(g);
                }

        case('K'):
                {
                    kosaraju_algorithm(g);
                 }       
    }

node **comp_arr=(node**)malloc((g->comp+1)*sizeof(node*));
for(int i=0;i<=g->comp;i++)
comp_arr[i]=NULL;


for(int i=0;i<g->vertices;i++)
insert_link(&comp_arr[g->component[i]],i);
printf("\n");
for(int i=1;i<g->comp;i++)
    {
        printf("Component %d: ",i);
        node *copy=comp_arr[i];
        while(copy!=NULL)
        {
            printf("%d ",copy->data);
            copy=copy->next;
        }
        printf("\n");
    }
    printf("The following are SCC recommendations\n");
for(int i=1;i<g->comp;i++)
{   
    node *current,*nxt;
    current=comp_arr[i];
    
    while(current!=NULL)
    {   
        nxt=current->next;
        printf("%d: ",current->data);
        while(nxt!=NULL)
        {
            if(mat[current->data][nxt->data]==0 && mat[nxt->data][current->data]==0)
            {
                printf("%d ",nxt->data);
            }
            nxt=nxt->next;
        }
        printf("\n");
        current=current->next;

    }
}
 printf("The following are add back recommendations:\n");
for(int i=1;i<g->comp;i++)
{   
    node *current,*nxt;
    current=comp_arr[i];
    
    while(current!=NULL)
    {   
        nxt=comp_arr[i];
        printf("%d: ",current->data);
        while(nxt!=NULL)
        {
            if(mat[current->data][nxt->data]==0 && mat[nxt->data][current->data]==1)
            {
                printf("%d ",nxt->data);
            }
            nxt=nxt->next;
        }
        printf("\n");
        current=current->next;

    }
}

    return 0;

}    
