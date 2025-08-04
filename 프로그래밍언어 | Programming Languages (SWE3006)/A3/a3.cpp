#include <chrono>
#include <iostream>

using namespace std;

const int SIZE = 1000;
const int ITERATIONS = 1000000000;

void static_array() {
    static int static_arr[SIZE];
}

void stack_array() {
    int stack_arr[SIZE];
}

void heap_array() {
    int *heap_arr = new int[SIZE];
}

void runtime_test(void (*func)(), const string &name) {
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i)
        func();
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << name << ": " << elapsed.count() << " ms" << endl;
}

int main() {
    runtime_test(static_array, "Static Array");
    runtime_test(stack_array, "Stack Array");
    runtime_test(heap_array, "Heap Array");

    return 0;
}