#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INSTRUCTIONS 10000
#define INT_MAX 2147483647
#define INPUT_FILE "mst.in"
#define OUTPUT_FILE "mst.out"

// structures ================================================================
typedef struct EDGE{
    int dest; //destination vertex
    int weight; //weight of edge
    struct EDGE* next; //next edge
}Edge;

typedef struct ADJLIST{
    Edge* head; //head of adjacency list
} AdjList;

typedef struct GRAPH{
    int num_nodes; //number of vertices
    int num_edges; //number of edges
    AdjList* adj_list; //adjacency list
    int **adj_matrix;  //adjacency matrix
} Graph;

typedef struct VERTEX{
    int name; //name of vertex
    int key; //key value used in prim's algorithm
    int inMST; //checks if vertex is in MST
    struct VERTEX* parent;
} Vertex;

typedef struct HEAP{
    int size; //size of heap
    int* pos; //stores position of vertex in heap
    Vertex **vertex; //stores vertices in heap
}Heap;

typedef struct INSTR{
    char instruction;
    int node1;
    int node2;
    int weight;
}Instr;

// heap functions ===========================================================
void swap_int(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_vertex(Vertex** a, Vertex** b){
    Vertex* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(Heap* heap, int index){
    int left = index * 2+1; // left child index
    int right = index * 2+2; //right child index
    int min = index;

    if(left < heap->size && heap->vertex[left]->key < heap->vertex[min]->key){
        min = left;
    }
    if(right < heap->size && heap->vertex[right]->key < heap->vertex[min]->key){
        min = right;
    }

    if(min != index){
        swap_vertex(&heap->vertex[index], &heap->vertex[min]);
        swap_int(&heap->pos[heap->vertex[index]->name], &heap->pos[heap->vertex[min]->name]);

        heapifyDown(heap, min);
    }
}

void heapifyUp(Heap* heap, int index){
    while(index > 0 && heap->vertex[(index-1)/2]->key > heap->vertex[index]->key){
        swap_vertex(&heap->vertex[index], &heap->vertex[(index-1)/2]);
        swap_int(&heap->pos[heap->vertex[index]->name], &heap->pos[heap->vertex[(index-1)/2]->name]);

        index = (index-1)/2;
    }
}

//get the minimum vertex from heap
Vertex* extractMin(Heap* heap){
    if(heap->size == 0){
        return NULL;
    }

    Vertex* min = heap->vertex[0];

    swap_vertex(&heap->vertex[0], &heap->vertex[heap->size-1]);
    swap_int(&heap->pos[heap->vertex[0]->name], &heap->pos[heap->vertex[heap->size-1]->name]);

    heap->size--;
    heapifyDown(heap, 0);

    return min;
}

void initHeap(int num_nodes, Heap* heap){
    heap->size = num_nodes;
    heap->pos = malloc(sizeof(int) * num_nodes);
    heap->vertex = malloc(sizeof(Vertex*) * num_nodes);
}

// prim's algorithm =================================================================
int prim(Graph* graph, Heap* queue){
    if(graph->num_edges == 0){
        return -1;
    }

    //initialize all vertices; key values are all set to infinity
    for(int i = 0; i < graph->num_nodes; i++){
        queue->vertex[i] = malloc(sizeof(Vertex));
        queue->vertex[i]->name = i;
        queue->vertex[i]->key = INT_MAX;
        queue->vertex[i]->key = INT_MAX;
        queue->vertex[i]->inMST = 0;
        queue->vertex[i]->parent = NULL;
        queue->pos[i] = i;
    }

    queue->vertex[0]->key = 0;
    queue->vertex[0]->key = 0;
    queue->vertex[0]->parent = NULL; //first element doesn't have a parent
    queue->vertex[0]->inMST = 1;

    int total_weight = 0;

    while(queue->size != 0){
        Vertex* min_node = extractMin(queue);
        //if it doesn't have a parent and it's not the first element, then it's disconnected
        if(min_node->parent == NULL && min_node->name != 0){
            return -1;
        }

        int u = min_node->name;
        min_node->inMST = 1;
        total_weight += min_node->key;

        Edge* adjlist = graph->adj_list[u].head;
        while(adjlist != NULL){
            if(queue->vertex[queue->pos[adjlist->dest]]->inMST == 0 && queue->pos[adjlist->dest] < queue->size && adjlist->weight < queue->vertex[queue->pos[adjlist->dest]]->key){
                queue->vertex[queue->pos[adjlist->dest]]->parent = min_node;
                queue->vertex[queue->pos[adjlist->dest]]->key = adjlist->weight;
                heapifyUp(queue, queue->pos[adjlist->dest]);
            }
            adjlist = adjlist->next;
        }
    }
    return total_weight;
}

// input instruction functions ===========================================================
void insertEdge(Instr instr, Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] == 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = instr.weight;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = instr.weight;
        graph->num_edges++;

        //insert into adj_list
        Edge* new_edge = malloc(sizeof(Edge));
        new_edge->dest = instr.node2-1;
        new_edge->weight = instr.weight;
        new_edge->next = graph->adj_list[instr.node1-1].head;
        graph->adj_list[instr.node1-1].head = new_edge;

        //insert into adj_list on the other side
        Edge* new_edge2 = malloc(sizeof(Edge));
        new_edge2->dest = instr.node1-1;
        new_edge2->weight = instr.weight;
        new_edge2->next = graph->adj_list[instr.node2-1].head;
        graph->adj_list[instr.node2-1].head = new_edge2;
    }
}

void changeWeight(Instr instr, Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] > 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = instr.weight;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = instr.weight;

        //change weight in adj_list
        Edge* temp = graph->adj_list[instr.node1-1].head;
        while(temp != NULL){
            if(temp->dest == instr.node2-1){
                temp->weight = instr.weight;
                break;
            }
            temp = temp->next;
        }

        //change weight in adj_list on the other side
        temp = graph->adj_list[instr.node2-1].head;
        while(temp != NULL){
            if(temp->dest == instr.node1-1){
                temp->weight = instr.weight;
                break;
            }
            temp = temp->next;
        }
    }
}

void deleteEdge(Instr instr , Graph* graph){
    if(graph->adj_matrix[instr.node1-1][instr.node2-1] > 0){
        graph->adj_matrix[instr.node1-1][instr.node2-1] = 0;
        graph->adj_matrix[instr.node2-1][instr.node1-1] = 0;
        graph->num_edges--;

        //delete from adj_list
        Edge* temp = graph->adj_list[instr.node1-1].head;
        Edge* prev = NULL;
        while(temp != NULL){
            if(temp->dest == instr.node2-1){
                if(prev == NULL){
                    graph->adj_list[instr.node1-1].head = temp->next;
                }
                else{
                    prev->next = temp->next;
                }
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }

        //delete from adj_list on the other side
        temp = graph->adj_list[instr.node2-1].head;
        prev = NULL;
        while(temp != NULL){
            if(temp->dest == instr.node1-1){
                if(prev == NULL){
                    graph->adj_list[instr.node2-1].head = temp->next;
                }
                else{
                    prev->next = temp->next;
                }
                free(temp);
                break;
            }
            prev = temp;
            temp = temp->next;
        }
    }
}

void findMST(Graph* graph, FILE* output_file){
    Heap queue;
    initHeap(graph->num_nodes, &queue);

    int mst_weight = prim(graph, &queue);
    
    //print total weight
    if(mst_weight == -1){
        fprintf(output_file, "Disconnected\n");
    }
    else {
        fprintf(output_file, "%d\n", mst_weight);
    }
}


// main ======================================================================
int main(){
    // Checking run time
    // int TIME = 0;
    // clock_t start = clock();

    FILE *input_file = fopen(INPUT_FILE, "r");
    FILE *output_file = fopen(OUTPUT_FILE, "w");

    if (input_file == NULL || output_file == NULL) {
        printf("Error: File not found.\n");
        return EXIT_FAILURE;
    }

    //scan the number of nodes
    int num_nodes = 0;
    fscanf(input_file, "%d", &num_nodes);

    Instr instr_list[MAX_INSTRUCTIONS];
    
    //create new graph
    Graph graph;
    graph.num_nodes = num_nodes;
    graph.num_edges = 0;
    graph.adj_matrix = malloc(sizeof(int*) * (num_nodes));
    graph.adj_list = malloc(sizeof(AdjList) * num_nodes);
    for(int i = 0; i < num_nodes; i++){
        graph.adj_list[i].head = NULL;
        graph.adj_matrix[i] = calloc(num_nodes, sizeof(int));
    }

    //scan the instructions
    char instr_name[20];
    int list_index = 0;
    int start_index = 0;
    while(fscanf(input_file, "%s", instr_name) == 1){
        //findMST instruction
        if(instr_name[0] == 'f'){
            for(int i = start_index; i < list_index; i++){
                //insertEdge
                if(instr_list[i].instruction == 'i'){
                    insertEdge(instr_list[i], &graph);
                }
                //changeWeight
                else if(instr_list[i].instruction == 'c'){
                    changeWeight(instr_list[i], &graph);
                }
                //deleteEdge
                else if(instr_list[i].instruction == 'd'){
                    deleteEdge(instr_list[i], &graph);
                }
            }
            //findMST
            findMST(&graph, output_file);
            start_index = list_index;
        } //deleteEdge instruction
        else if (instr_name[0] == 'd'){
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d", &instruction.node1, &instruction.node2);
            instr_list[list_index++] = instruction;
        } //changeWeight & insertEdge instruction
        else{
            Instr instruction;
            instruction.instruction = instr_name[0];
            fscanf(input_file, "%d %d %d", &instruction.node1, &instruction.node2, &instruction.weight);
            instr_list[list_index++] = instruction;
        }
    }

    fclose(input_file);
    fclose(output_file);

    // TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    // printf("TIME : %d ms\n", TIME);
}