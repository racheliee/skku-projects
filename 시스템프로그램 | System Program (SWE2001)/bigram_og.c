#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "shakespeare.txt"
#define MAX_WORD_SIZE 100
#define BUCKET_SIZE 1021
#define MAX_BIGRAMS 100000000

// structs ==============================================
typedef struct Node{
    char* word1;
    char* word2;
    int count;
    struct Node* next;
} Node;

//wrapper functions ======================================
int string_length(char* s){
    return strlen(s);
}

int string_compare(char* s1, char* s2){
    return strcmp(s1, s2);
}

void string_copy(char* dest, char* src){
    strcpy(dest, src);
}

// helper functions ======================================
// Each word is read from the file and converted to lowercase. Our initial version used the function lower1 (Figure 5.7), 
// which we know to have quadratic run time due to repeated calls to strlen.
void lower_case(char* s){
    for(long i = 0; i < string_length(s); i++){
        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] -= ('A' - 'a');
        }
    }
}

// remove punctuation from a word
void remove_punctuation(char* word){
    char no_punct[string_length(word)+1];
    int index = 0;
    int i = 0;

    for(i = 0; i < string_length(word); i++){
        if(!ispunct(word[i])){
            no_punct[index] = word[i];
            index++;
        }
    }

    no_punct[index] = '\0';
    string_copy(word, no_punct);
}

// functions ============================================
//A hash function is applied to the string to create a number between 0 and s − 1, for a hash table with s buckets. 
//Our initial function simply summed the ASCII codes for the characters modulo s.
int hash_function(char* word1, char* word2){
    int ascii_sum = 0;

    for(int i = 0; i < string_length(word1); i++){
        ascii_sum += word1[i];
    }
    for(int i = 0; i < string_length(word2); i++){
        ascii_sum += word2[i];
    }

    return ascii_sum % BUCKET_SIZE;
}

//insert a new bigram into the hashtable
void insert(Node** hashtable, char* first_w, char* second_w){
    //create a new node
    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->word1 = (char*)malloc(sizeof(char) * (string_length(first_w)+1));
    new_node->word2 = (char*)malloc(sizeof(char) * (string_length(second_w)+1));  
    string_copy(new_node->word1, first_w);
    string_copy(new_node->word2, second_w);

    new_node->count = 1;
    new_node->next = NULL;

    int hash_value = hash_function(first_w, second_w); 

    if(hashtable[hash_value] == NULL){
        hashtable[hash_value] = new_node;
    }
    else{
        Node* temp = hashtable[hash_value];

        while(temp->next != NULL){
            // if the bigram already exists, increment the count
            if(string_compare(temp->word1, first_w) == 0 && string_compare(temp->word2, second_w) == 0){
                temp->count++;
                return;
            }

            // otherwise, go to the next node
            temp = temp->next;
        }
        // if the bigram already exists and is at the end of the linked list, increment the count
        if(string_compare(temp->word1, first_w) == 0 && string_compare(temp->word2, second_w) == 0){
            temp->count++;
            return;
        }
        // if the bigram doesn't exist, add it to the end of the linked list
        temp->next = new_node;
    }
}

// reads the input file and stores it into the hashtable
void read_file_and_hash(Node** hashtable){
    FILE *input_file = fopen(FILE_NAME, "r");

    if(input_file == NULL){
        printf("Error: File not found\n");
        exit(1);
    }

    // change this to static later
    char* first_w = malloc(sizeof(char) * MAX_WORD_SIZE);
    char* second_w = malloc(sizeof(char) * MAX_WORD_SIZE);

    //scan first word
    if(fscanf(input_file, "%99s", first_w) == 0){
        printf("Error: File is empty\n");
        exit(1);
    }
     
    while(fscanf(input_file, "%99s", second_w) == 1){
        lower_case(first_w);
        lower_case(second_w);
        remove_punctuation(first_w);
        remove_punctuation(second_w);

        insert(hashtable, first_w, second_w);
        
        string_copy(first_w, second_w); //change the first word to the second word
    }
}

//unhashes all values into an array
void hash_to_array(Node** hashtable, Node** sorted_bigrams, int* size){
    int sorted_index = 0;
    // add all hash elements to the array
    for(int i = 0; i < BUCKET_SIZE; i++){
        Node* current_bigram = hashtable[i];

        while(current_bigram != NULL){
            sorted_bigrams[sorted_index] = current_bigram;
            sorted_index++;
            current_bigram = current_bigram->next;
        }
    }

    *size = sorted_index;
    printf("Total bigrams: %d\n", *size);
}

// sort the bigrams in the hashtable in descending order
void insertion_sort(Node** hashtable, Node** sorted_bigrams){
    int array_size = 0;
    hash_to_array(hashtable, sorted_bigrams, &array_size);

    // sort the array with insertion sort
    Node* key;

    for(int i = 1; i < array_size; i++){
        key = sorted_bigrams[i];
        int j = i - 1;

        while(j >= 0 && sorted_bigrams[j]->count < key->count){
            sorted_bigrams[j + 1] = sorted_bigrams[j];
            j = j - 1;
        }
        sorted_bigrams[j + 1] = key;
    }

}


// main function ========================================
int main(){
    //initialize hash table, an array of pointers to nodes
    Node** hashtable = (Node**)malloc(sizeof(Node*) * BUCKET_SIZE);
    for(int i = 0; i < BUCKET_SIZE; i++){
        hashtable[i] = NULL;
    }

    read_file_and_hash(hashtable);

    //create array to store sorted bigrams
    Node** sorted_bigrams = (Node**)malloc(sizeof(Node*) * MAX_BIGRAMS);
    for(int i = 0; i < MAX_BIGRAMS; i++){
        sorted_bigrams[i] = NULL;
    }
    insertion_sort(hashtable, sorted_bigrams);

    //print results
    printf("Top 10 bigrams: \n");
    for(int i = 0; i < 10; i++){
        printf("#%d: %s %s %d\n", i+1, sorted_bigrams[i]->word1, sorted_bigrams[i]->word2, sorted_bigrams[i]->count);
    
    }

    return 0;
}