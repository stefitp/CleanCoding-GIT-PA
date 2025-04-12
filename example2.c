#include <stdio.h>
#include <stdlib.h>

// structura pentru nod din lista de adiacenta
typedef struct Node {
    int data;
    struct Node *next;
} NODE;

// structura pentru graf
typedef struct Graph {
    int vertices;
    int *visited;
    NODE **adjacency_lists;
} GPH;

// creeaza un nod nou
NODE *create_node(int v) {
    NODE *new_node = malloc(sizeof(NODE));
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

// creeaza un graf cu un numar dat de noduri
GPH *create_graph(int vertices) {
    GPH *graph = malloc(sizeof(GPH));
    graph->vertices = vertices;
    graph->adjacency_lists = malloc(vertices * sizeof(NODE *));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// adauga o muchie neorientata intre doua noduri
void add_edge(GPH *graph, int src, int dest) {
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

// insereaza muchii in graf pe baza inputului
void insert_edges(int nr_of_vertices, int nr_of_edges, GPH *graph) {
    int src, dest;
    printf("adauga %d muchii (de la 0 la %d):\n", nr_of_edges, nr_of_vertices - 1);
    for (int i = 0; i < nr_of_edges; i++) {
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

// verifica daca coada e goala
int is_empty(NODE *queue) {
    return queue == NULL;
}

// adauga un element la finalul cozii
void enqueue(NODE **queue, int data) {
    NODE *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        NODE *temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// scoate si returneaza primul element din coada
int dequeue(NODE **queue) {
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

// reseteaza vectorul de noduri vizitate
void wipe_visited_list(GPH *graph, int nr_of_vertices) {
    for (int i = 0; i < nr_of_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// parcurgere in adancime (dfs)
void DFS(GPH *graph, int vertex_nr) {
    NODE *temp = graph->adjacency_lists[vertex_nr];
    graph->visited[vertex_nr] = 1;
    printf("%d ", vertex_nr);

    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// parcurgere in latime (bfs)
void BFS(GPH *graph, int start) {
    NODE *queue = NULL;
    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE *temp = graph->adjacency_lists[current];
        while (temp != NULL) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

// afiseaza lista de adiacenta a grafului
void print_graph(GPH *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        NODE *temp = graph->adjacency_lists[i];
        printf("nodul %d: ", i);
        while (temp) {
            printf("%d -> ", temp->data);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    int nr_of_vertices, nr_of_edges, starting_vertex;

    printf("cate noduri are graful? ");
    scanf("%d", &nr_of_vertices);

    printf("cate muchii are graful? ");
    scanf("%d", &nr_of_edges);

    GPH *graph = create_graph(nr_of_vertices);

    insert_edges(nr_of_vertices, nr_of_edges, graph);

    printf("de unde plecam in dfs? ");
    scanf("%d", &starting_vertex);
    printf("parcurgere cu dfs: ");
    DFS(graph, starting_vertex);

    wipe_visited_list(graph, nr_of_vertices);

    printf("\n");

    printf("de unde plecam in bfs? ");
    scanf("%d", &starting_vertex);
    printf("parcurgere cu bfs: ");
    BFS(graph, starting_vertex);

    printf("\n");
    return 0;
}
