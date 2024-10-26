#include <shared_mutex>
using namespace std;

class Llist_node {
 public:  
  Llist_node(int d) {   
    data = d;
    next = NULL;
  }
  
  int data;
  Llist_node *next;
};

class CSE113_Stack {
 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    m.lock(); // lock before accessing shared data
    if (start == NULL) {
      m.unlock(); // unlock after accessing shared data
      return -1;
    }

    if (start->next == NULL) {
      int ret = start->data;
      delete start;
      start = NULL;
      m.unlock();  // unlock after accessing shared data
      return ret;
    }

    Llist_node *current = start->next;
    Llist_node *previous = start;
    while (current->next != NULL) {
      previous = current;
      current = current->next;
    }

    int ret = current->data;
    previous->next = NULL;
    delete current;

    m.unlock();  // unlock after accessing shared data
    return ret;
  }

  
  int peek() {
    m.lock_shared(); // use a shared lock because we are only reading
    if (start == NULL) {
      m.unlock_shared();  // unlock after accessing shared data
      return -1;
    }
    
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    m.unlock_shared();  // unlock after accessing shared data
    return current->data;    
  }
  
  void push(int p) {
    m.lock(); // lock before accessing shared data
    if (start == NULL) {
      start = new Llist_node(p);
      m.unlock();  // unlock after accessing shared data
      return;
    }

    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    current->next = new Llist_node(p);

    m.unlock();  // unlock after accessing shared data
  }

 private:
  Llist_node *start;  
  shared_mutex m; // shared mutex
};

