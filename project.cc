// scc_algorithms.cpp
// Program to generate a random directed graph and find its Strongly Connected Components (SCCs)
// using either Tarjan's Algorithm or Kosaraju's Algorithm.
//
// Conversion: Original C code rewritten in C++ (with modern features).
// Author: Converted by ChatGPT
//
// Compile with:
// g++ -std=c++17 -O2 scc_algorithms.cpp -o scc_algorithms
//
// Usage:
// ./scc_algorithms
//   - The program generates a random directed graph (with adjacency list + adjacency matrix).
//   - Displays the graph.
//   - Prompts user to choose between Tarjan's or Kosaraju's algorithm.
//   - Displays SCCs and recommendations based on graph structure.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Global variables for timestamps in DFS
int t = 0;   // global DFS time for discovery/finish in Kosaraju
int tt = 0;  // global DFS time for discovery in Tarjan

// Global adjacency matrix (for easy edge lookup when printing recommendations)
std::vector<std::vector<int>> mat;

// Constants
#define MAX_VERTICES 100   // Maximum number of vertices
#define MAX_EDGES 1000     // Maximum number of edges

//-----------------------------------------
// Stack class (like the C struct but C++-ified)
//-----------------------------------------
class Stack {
public:
    Stack(int capacity) : capacity_(capacity), top_(-1) {
        array_.resize(capacity_);
    }
    bool isFull() const { return top_ == capacity_ - 1; }
    bool isEmpty() const { return top_ == -1; }
    void push(int item) {
        if (isFull()) return;
        array_[++top_] = item;
    }
    int pop() {
        if (isEmpty()) return -1;
        return array_[top_--];
    }
    int top() const {
        if (isEmpty()) return -1;
        return array_[top_];
    }
private:
    int capacity_;          // max size of stack
    int top_;               // current top index
    std::vector<int> array_; // stack storage
};

//-----------------------------------------
// Linked-list node for adjacency list
//-----------------------------------------
struct Node {
    int data;
    Node* next;
    Node(int d) : data(d), next(nullptr) {}
};

//-----------------------------------------
// Graph structure
//-----------------------------------------
struct Graph {
    int vertices;                // number of vertices
    int edges;                   // number of edges
    std::vector<Node*> adjlist;  // adjacency list
    std::vector<int> dis;        // discovery times
    std::vector<int> finish;     // finish times
    std::vector<int> component;  // component assignment (SCC id)
    int comp;                    // number of components
    Graph() : vertices(0), edges(0), comp(1) {}
};

//-----------------------------------------
// Utility function
//-----------------------------------------
inline int min_two(int n, int m) {
    return (n < m) ? n : m;
}

//-----------------------------------------
// Insert an edge into adjacency list
//-----------------------------------------
void insert_link(Node** root, int key) {
    Node* current = new Node(key);
    if (*root == nullptr) {
        *root = current;
        return;
    }
    Node* prev = *root;
    while (prev->next != nullptr) prev = prev->next;
    prev->next = current;
}

//-----------------------------------------
// Generate random directed graph
//-----------------------------------------
void rand_graph(Graph* g) {
    srand(static_cast<unsigned>(time(nullptr)));

    int numVertices = rand() % MAX_VERTICES + 1;
    int numEdges = rand() % MAX_EDGES + 1;

    // Ensure at least 2 vertices
    if (numVertices < 2) numVertices = 2;

    // Ensure a simple graph (no multiple edges/self-loops beyond capacity)
    if (numEdges > (numVertices * (numVertices - 1)) / 2)
        numEdges = (numVertices * (numVertices - 1)) / 2;

    // initialize adjacency matrix
    mat.assign(numVertices, std::vector<int>(numVertices, 0));

    g->edges = numEdges;
    g->vertices = numVertices;
    g->adjlist.assign(numVertices, nullptr);
    g->comp = 1;

    g->component.assign(g->vertices, 0);
    g->dis.assign(g->vertices, 0);
    g->finish.assign(g->vertices, 0);

    // Generate random edges
    for (int i = 0; i < numEdges; ++i) {
        int source = rand() % numVertices;
        int destination = rand() % numVertices;

        // Ensure no self-loops or duplicate edges
        while (source == destination || mat[destination][source] == 1 || mat[source][destination] == 1) {
            source = rand() % numVertices;
            destination = rand() % numVertices;
        }

        mat[source][destination] = 1;
        insert_link(&g->adjlist[source], destination);
    }
}

//-----------------------------------------
// DFS for Kosaraju (first pass)
//-----------------------------------------
void top_dfs(Graph* g, std::vector<int>& arr, int u, Stack* s) {
    arr[u] = 1;
    ++t;
    g->dis[u] = t;
    Node* copy = g->adjlist[u];
    while (copy != nullptr) {
        if (arr[copy->data] == 0) {
            top_dfs(g, arr, copy->data, s);
        }
        copy = copy->next;
    }
    ++t;
    g->finish[u] = t;
    s->push(u);
    arr[u] = 2;
}

//-----------------------------------------
// Topological sort (Kosaraju step 1)
//-----------------------------------------
void topological_sort(Graph* g, Stack* s) {
    std::vector<int> arr(g->vertices, 0);
    for (int i = 0; i < g->vertices; ++i) {
        if (arr[i] == 0) {
            top_dfs(g, arr, i, s);
        }
    }
}

//-----------------------------------------
// DFS for Kosaraju (second pass)
//-----------------------------------------
void dfs(Graph* g, std::vector<int>& arr, int u, Stack* /*s*/) {
    arr[u] = 1;
    Node* copy = g->adjlist[u];
    while (copy != nullptr) {
        if (arr[copy->data] == 0) {
            g->component[copy->data] = g->component[u];
            dfs(g, arr, copy->data, nullptr);
        }
        copy = copy->next;
    }
    arr[u] = 2;
}

//-----------------------------------------
// DFS traversal for Kosaraju (step 3)
//-----------------------------------------
void dfs_traversal(Graph* g, Stack* s) {
    g->comp = 1;
    std::vector<int> vis(g->vertices, 0);

    while (!s->isEmpty()) {
        int u = s->pop();
        if (u < 0 || u >= g->vertices) continue;
        if (vis[u] == 0) {
            g->component[u] = g->comp;
            dfs(g, vis, u, s);
            g->comp++;
        }
    }
}

//-----------------------------------------
// Reverse graph (Kosaraju step 2)
//-----------------------------------------
void rev_graph(Graph* g) {
    Graph* temp = new Graph();
    temp->vertices = g->vertices;
    temp->edges = g->edges;
    temp->adjlist.assign(g->vertices, nullptr);

    // Copy adjacency list (same direction)
    for (int i = 0; i < g->vertices; ++i) {
        Node* copy = g->adjlist[i];
        while (copy != nullptr) {
            insert_link(&temp->adjlist[i], copy->data);
            copy = copy->next;
        }
    }

    // Clear original adjacency lists
    for (int i = 0; i < g->vertices; ++i) {
        Node* cur = g->adjlist[i];
        while (cur != nullptr) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
        g->adjlist[i] = nullptr;
    }

    // Build reversed edges
    for (int i = 0; i < g->vertices; ++i) {
        Node* copy = temp->adjlist[i];
        while (copy != nullptr) {
            insert_link(&g->adjlist[copy->data], i);
            copy = copy->next;
        }
    }

    // Cleanup temp
    for (int i = 0; i < temp->vertices; ++i) {
        Node* cur = temp->adjlist[i];
        while (cur != nullptr) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }
    delete temp;
}

//-----------------------------------------
// Display adjacency list
//-----------------------------------------
void display(Graph* g) {
    for (int i = 0; i < g->vertices; ++i) {
        std::cout << "Vertex " << i << ": ";
        Node* copy = g->adjlist[i];
        while (copy != nullptr) {
            std::cout << copy->data << ' ';
            copy = copy->next;
        }
        std::cout << '\n';
    }
}

//-----------------------------------------
// Kosaraju's Algorithm
//-----------------------------------------
void kosaraju_algorithm(Graph* g) {
    Stack s(g->vertices);
    topological_sort(g, &s); // Step 1: DFS finishing times
    rev_graph(g);            // Step 2: Reverse graph
    dfs_traversal(g, &s);    // Step 3: DFS in order of finishing times
}

//-----------------------------------------
// Tarjan's DFS helper
//-----------------------------------------
void tarjan_dfs(Graph* g, int u, std::vector<int>& disc, std::vector<int>& low,
                Stack* st, std::vector<int>& in_stack) {
    disc[u] = low[u] = tt++;
    in_stack[u] = 1;
    st->push(u);
    Node* copy = g->adjlist[u];
    while (copy != nullptr) {
        int v = copy->data;
        if (disc[v] == -1) {
            tarjan_dfs(g, v, disc, low, st, in_stack);
            low[u] = min_two(low[u], low[v]);
        } else if (in_stack[v] == 1) {
            low[u] = min_two(low[u], disc[v]);
        }
        copy = copy->next;
    }

    // If u is the root of an SCC
    if (low[u] == disc[u]) {
        while (st->top() != u) {
            int m = st->pop();
            if (m >= 0 && m < g->vertices) {
                in_stack[m] = 0;
                g->component[m] = g->comp;
            }
        }
        int m = st->pop();
        if (m >= 0 && m < g->vertices) {
            in_stack[m] = 0;
            g->component[m] = g->comp;
        }
        g->comp++;
    }
}

//-----------------------------------------
// Tarjan's Algorithm
//-----------------------------------------
void tarjan_algorithm(Graph* g) {
    std::vector<int> disc(g->vertices, -1);
    std::vector<int> low(g->vertices, -1);
    std::vector<int> in_stack(g->vertices, 0);
    g->comp = 1;
    Stack st(g->vertices);

    for (int i = 0; i < g->vertices; ++i) {
        if (disc[i] == -1) {
            tarjan_dfs(g, i, disc, low, &st, in_stack);
        }
    }
}

//-----------------------------------------
// Main
//-----------------------------------------
int main() {
    Graph* g = new Graph();
    rand_graph(g);

    // Display graph
    std::cout << "\nRandom Directed graph (Adjacency list)\n";
    display(g);

    // Ask user
    char c1;
    std::cout << "Enter 'T' for Tarjan's algorithm and 'K' for Kosaraju's algorithm : ";
    std::cin >> c1;

    switch (c1) {
    case 'T':
    case 't':
        tarjan_algorithm(g);
        break;
    case 'K':
    case 'k':
        kosaraju_algorithm(g);
        break;
    default:
        std::cout << "Unknown option. Running Tarjan by default.\n";
        tarjan_algorithm(g);
        break;
    }

    // Build array of components for printing
    std::vector<Node*> comp_arr(g->comp + 1, nullptr);
    for (int i = 0; i < g->vertices; ++i) {
        if (g->component[i] >= 0 && g->component[i] <= g->comp) {
            insert_link(&comp_arr[g->component[i]], i);
        }
    }

    // Print SCCs
    std::cout << '\n';
    for (int i = 1; i < g->comp; ++i) {
        std::cout << "Component " << i << ": ";
        Node* copy = comp_arr[i];
        while (copy != nullptr) {
            std::cout << copy->data << ' ';
            copy = copy->next;
        }
        std::cout << '\n';
    }

    // Recommendations (nodes in same SCC without edges between them)
    std::cout << "The following are SCC recommendations\n";
    for (int i = 1; i < g->comp; ++i) {
        Node* current = comp_arr[i];
        while (current != nullptr) {
            Node* nxt = current->next;
            std::cout << current->data << ": ";
            while (nxt != nullptr) {
                if (mat[current->data][nxt->data] == 0 && mat[nxt->data][current->data] == 0) {
                    std::cout << nxt->data << ' ';
                }
                nxt = nxt->next;
            }
            std::cout << '\n';
            current = current->next;
        }
    }

    // Recommendations (add-back edges)
    std::cout << "The following are add back recommendations:\n";
    for (int i = 1; i < g->comp; ++i) {
        Node* current = comp_arr[i];
        while (current != nullptr) {
            Node* nxt = comp_arr[i];
            std::cout << current->data << ": ";
            while (nxt != nullptr) {
                if (mat[current->data][nxt->data] == 0 && mat[nxt->data][current->data] == 1) {
                    std::cout << nxt->data << ' ';
                }
                nxt = nxt->next;
            }
            std::cout << '\n';
            current = current->next;
        }
    }

    // Cleanup
    for (int i = 0; i < g->vertices; ++i) {
        Node* cur = g->adjlist[i];
        while (cur != nullptr) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }
    for (Node* head : comp_arr) {
        Node* cur = head;
        while (cur != nullptr) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }

    delete g;
    return 0;
}
