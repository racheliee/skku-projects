#include <stdio.h>
#include <time.h>

#define MAX_SIZE 500000

// min-max heap structure (also works for max-min heap)
typedef struct MinMaxHeap {
    int arr[MAX_SIZE / 2 + 1];
    int size;
    int min_max; // for min-max heap = 1 and max-min heap = 0
} Heap;

typedef struct PriorityQueue {
    Heap lesser;  // max-min heap which stores the lesser half of values
    Heap greater; // min-max heap which stores the greater half of values
} PQ;

// set priority queue as a global variable
PQ pq;

/*helper functions needed for heap functions*/
// swaps the two elements passed
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// returns the level of the heap based on the index passed
int get_tree_level(int index) {
    int level = 0;
    int node_index = index;

    while (node_index != 0) {
        if (node_index % 2 == 0) {
            node_index--;
        }
        node_index /= 2;
        level++;
    }

    return level;
}

// returns 1 if the node at given index is on a min level of the heap
int at_min_level(Heap *heap, int index) {
    if (heap->min_max == 1 && get_tree_level(index) % 2 == 0)
        return 1;
    if (heap->min_max == 0 && get_tree_level(index) % 2 == 1)
        return 1;
    return 0;
}

// returns the index of the smallest child or grandchild of given index node
int smallest_descendant_index(Heap *heap, int index) {
    if (heap->size - 1 < 2 * index + 1)
        return -1;

    int smallest_index = 2 * index + 1;
    int smallest_descendant = heap->arr[smallest_index];

    // checking other child
    if (2 * index + 2 <= heap->size - 1 && smallest_descendant > heap->arr[2 * index + 2]) {
        smallest_index = 2 * index + 2;
        smallest_descendant = heap->arr[smallest_index];
    }

    // checking grandchildren
    for (int i = 3; i <= 6; i++) {
        if (4 * index + i <= heap->size - 1 && smallest_descendant > heap->arr[4 * index + i]) {
            smallest_index = 4 * index + i;
            smallest_descendant = heap->arr[smallest_index];
        }
    }

    return smallest_index;
}

// returns the index of the largest child or grandchild of given index node
int largest_descendant_index(Heap *heap, int index) {
    if (heap->size - 1 < 2 * index + 1)
        return -1;

    int largest_index = 2 * index + 1;
    int largest_descendant = heap->arr[2 * index + 1];

    // checking other child
    if (2 * index + 2 <= heap->size - 1 && largest_descendant < heap->arr[2 * index + 2]) {
        largest_index = 2 * index + 2;
        largest_descendant = heap->arr[largest_index];
    }

    // checking grandchildren
    for (int i = 3; i <= 6; i++) {
        if (4 * index + i <= heap->size - 1 && largest_descendant < heap->arr[4 * index + i]) {
            largest_index = 4 * index + i;
            largest_descendant = heap->arr[largest_index];
        }
    }

    return largest_index;
}
// returns 1 is the grandchild index is a grandchild of the grandparent index of given heap
int is_a_grandchild(Heap *heap, int grandparent, int grandchild) {
    if (4 * grandparent + 3 <= grandchild && grandchild <= 4 * grandparent + 6) {
        return 1;
    }
    return 0;
}

// returns the maximum's index of the given heap
int find_heap_max_index(Heap *heap) {
    if (heap->size == 1 || heap->min_max == 0)
        return 0;
    if (heap->size == 2)
        return 1;
    else if (heap->arr[1] > heap->arr[2])
        return 1;
    else
        return 2;
}

// returns the minimums's index of the given heap
int find_heap_min_index(Heap *heap) {
    if (heap->size == 1 || heap->min_max == 1)
        return 0;
    if (heap->size == 2)
        return 1;
    else if (heap->arr[1] < heap->arr[2])
        return 1;
    else
        return 2;
}

/*heap functions*/
/*based on wikipedia's pseudocode on min-max heaps
pushes down the the element at given index down the given heap to its correct position*/
void push_down(Heap *heap, int index) {
    while (2 * index + 1 < heap->size) { // while node at index has a child
        int i = index;

        if (at_min_level(heap, i)) { // at min level
            index = smallest_descendant_index(heap, index);
            if (index == -1)
                break;

            if (heap->arr[index] < heap->arr[i]) {
                swap(&heap->arr[index], &heap->arr[i]);

                if (is_a_grandchild(heap, i, index)) {
                    if (heap->arr[index] > heap->arr[(index - 1) / 2]) {
                        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        } else {
            index = largest_descendant_index(heap, index);
            if (index == -1)
                break;

            if (heap->arr[index] > heap->arr[i]) {
                swap(&heap->arr[index], &heap->arr[i]);

                if (is_a_grandchild(heap, i, index)) {
                    if (heap->arr[index] < heap->arr[(index - 1) / 2]) {
                        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}

// deletes and returns the minimum element of the given heap
int pop_min(Heap *heap) {
    int min_index = find_heap_min_index(heap);
    int min = heap->arr[min_index];

    heap->size--;
    heap->arr[min_index] = heap->arr[heap->size];

    push_down(heap, min_index);

    return min;
}

// deletes and returns the maximum element of the given heap
int pop_max(Heap *heap) {
    int max_index = find_heap_max_index(heap);
    int max = heap->arr[max_index];

    heap->size--;
    heap->arr[max_index] = heap->arr[heap->size];

    push_down(heap, max_index);

    return max;
}

/*based on wikipedia's pseudocode on min-max heaps
pushes up the element at given index to the max levels of the given heap*/
void push_up_max(Heap *heap, int index) {
    if (index < 3)
        return;

    while (index >= 3 && heap->arr[(index - 3) / 4] < heap->arr[index]) {
        swap(&heap->arr[index], &heap->arr[(index - 3) / 4]);
        index = (index - 3) / 4;
    }
}

/*based on wikipedia's pseudocode on min-max heaps
pushes up the element at given index to the min levels of the given heap*/
void push_up_min(Heap *heap, int index) {
    if (index < 3)
        return;

    while (index >= 3 && heap->arr[index] < heap->arr[(index - 3) / 4]) {
        swap(&heap->arr[index], &heap->arr[(index - 3) / 4]);
        index = (index - 3) / 4;
    }
}

/*based on wikipedia's pseudocode on min-max heaps
pushes up the element at given index to the correct position of the given heap*/
void push_up(Heap *heap, int index) {
    if (index == 0)
        return;

    if (index > 0) {
        if (at_min_level(heap, index)) {
            if (heap->arr[index] > heap->arr[(index - 1) / 2]) {
                swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
                push_up_max(heap, (index - 1) / 2);
            } else {
                push_up_min(heap, index);
            }
        } else {
            if (heap->arr[index] < heap->arr[(index - 1) / 2]) {
                swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
                push_up_min(heap, (index - 1) / 2);
            } else {
                push_up_max(heap, index);
            }
        }
    }
}

// inserts given element to the given heap
void heap_insert(Heap *heap, int element) {
    heap->arr[heap->size] = element;
    push_up(heap, heap->size);
    heap->size++;
}

/*priority queue functions
Assumes that PQ has at least one element.
(Instances where PQ is empty will be handled in the main function)*/
// initializes priority queue (size of heaps set to 0 and indicates which heap is min-max)
void initPriorityQueue() {
    pq.lesser.size = 0;
    pq.greater.size = 0;

    pq.lesser.size = 0;
    pq.greater.min_max = 1;
}

// Inserts an integer element into the priority queue.
void insert(int element) {
    if (pq.greater.size == 0) {
        heap_insert(&pq.greater, element);
    } else if (element > pq.greater.arr[0]) {
        heap_insert(&pq.greater, element);
        if (pq.greater.size - pq.lesser.size == 2)
            heap_insert(&pq.lesser, pop_min(&pq.greater));

    } else {
        heap_insert(&pq.lesser, element);
        if (pq.lesser.size > pq.greater.size)
            heap_insert(&pq.greater, pop_max(&pq.lesser));
    }
}

// Fetches but does not remove the minimum element.
int find_min() {
    if (pq.lesser.size == 0)
        return pq.greater.arr[0];
    return pq.lesser.arr[find_heap_min_index(&pq.lesser)];
}

// Fetches but does not remove the median element.
int find_median() {
    if (pq.lesser.size == pq.greater.size)
        return pq.lesser.arr[0];
    else
        return pq.greater.arr[0];
}

// Fetches but does not remove the maximum element.
int find_max() {
    return pq.greater.arr[find_heap_max_index(&pq.greater)];
}

// Deletes and returns the minimum element.
int delete_min() {
    int min;
    if (pq.lesser.size == 0) {
        min = pop_min(&pq.greater);
        return min;
    }

    if (pq.greater.size == pq.lesser.size) {
        min = pop_min(&pq.lesser);
    } else {
        min = pop_min(&pq.lesser);
        heap_insert(&pq.lesser, pop_min(&pq.greater));
    }

    return min;
}

// Deletes and returns the median element.
int delete_median() {
    if (pq.lesser.size == pq.greater.size)
        return pop_max(&pq.lesser);
    else
        return pop_min(&pq.greater);
}

// Deletes and returns the maximum element.
int delete_max() {
    if (pq.greater.size == pq.lesser.size)
        heap_insert(&pq.greater, pop_max(&pq.lesser));
    return pop_max(&pq.greater);
}

/*============= main function ===============*/
int main() {
    // Checking run time
    // int TIME = 0;
    // clock_t start = clock();

    int num_operations;
    scanf("%d", &num_operations);

    initPriorityQueue();

    while (num_operations > 0) {
        char operation;
        scanf(" %c", &operation);

        int pq_size = pq.greater.size + pq.lesser.size;

        if (operation == 'I') { // Insert operation
            int num_target;
            scanf("%d", &num_target);

            insert(num_target);
        } else if (operation == 'D') { // Delete operation
            char target;
            scanf(" %c", &target);

            if (pq_size != 0) {
                if (target == 'M')
                    delete_min();
                else if (target == 'X')
                    delete_max();
                else if (target == 'E')
                    delete_median();
            }

        } // Find operation
        else if (operation == 'F') {
            char target;
            scanf(" %c", &target);

            if (pq_size != 0) {
                int found;
                if (target == 'M')
                    found = find_min();
                else if (target == 'X')
                    found = find_max();
                else if (target == 'E')
                    found = find_median();
                printf("%d\n", found);
            } else {
                printf("NULL\n");
            }
        }
        num_operations--;
    }

    // TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    // printf("TIME : %d ms\n", TIME);
    return 0;
}