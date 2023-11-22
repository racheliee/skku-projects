#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100000000
#define MAX_WORD_SIZE 100
#define BUCKET_SIZE 1021 //199999

// structs ==============================================
// change this to a single bigram word structure later
typedef struct Node{
    char* word1;
    char* word2;
    int count;
    struct Node* next;
} Node;

// helper functions ======================================
/*Each word is read from the file and converted to lowercase.Our initial version used the function lower1 (Figure 5.7), 
which we know to have quadratic run time due to repeated calls to strlen.*/
void lower_case1(char* s){
    for(long i ; i < strlen(s); i++){
        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] -= ('A' - 'a');
        }
    }
}

void lower_case2(char* s){
    long len = strlen(s);
    for(long i ; i < len; i++){
        if(s[i] >= 'A' && s[i] <= 'Z'){
            s[i] -= ('A' - 'a');
        }
    }
}

// remove punctuation from a word; apostrophes are not counted as punctuation
void remove_punctuation(char* word){
    for(int i = 0; i < strlen(word); i++){
        if(word[i] == '.' || word[i] == ',' || word[i] == '?' || word[i] == '!' || word[i] == ';' || word[i] == ':'){
            word[i] = '\0';
        }
    }
}

// functions ============================================

/*A hash function is applied to the string to create a number between 0 and s − 1, for a hash table with s buckets. 
Our initial function simply summed the ASCII codes for the characters modulo s.*/
int hash_function(char* word1, char* word2){
    int ascii_sum = 0;

    for(int i = 0; i < strlen(word1); i++){
        ascii_sum += word1[i];
    }
    for(int i = 0; i < strlen(word2); i++){
        ascii_sum += word2[i];
    }

    return ascii_sum % BUCKET_SIZE;
}

//insert a new bigram into the hashtable
void insert(Node** hashtable, char* first_w, char* second_w){
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->word1 = first_w;
    new_node->word2 = second_w;
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
            // compare in lower case and remove punctuation
            if(strcmp(temp->word1, first_w) == 0 && strcmp(temp->word2, second_w)){
                temp->count++;
                return;
            }
            // otherwise, go to the next node
            temp = temp->next;
        }
        // if the bigram doesn't exist, add it to the end of the linked list
        temp->next = new_node;
    }
}

// reads the input file and stores it into the hashtable
void read_file_and_hash(Node** hashtable){
    FILE *input_file = fopen("hamlet.txt", "r");

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
        lower_case1(first_w);
        lower_case1(second_w);
        remove_punctuation(first_w);
        remove_punctuation(second_w);

        printf("%s %s\n", first_w, second_w);
        insert(hashtable, first_w, second_w);
        
        strcpy(first_w, second_w); //change the first word to the second word
    }
}

// sort the bigrams in the hashtable
void sort(Node** hashtable, Node** sorted_bigrams){
    int sorted_index = 0;
    // add all hash elements to the array
    for(int i = 0; i < MAX_SIZE; i++){
        Node* current_bigram = hashtable[i];
        while(current_bigram != NULL){
            sorted_bigrams[sorted_index] = current_bigram;
            sorted_index++;
            current_bigram = current_bigram->next;
        }
    }
    //check
    for(int i = 0;i < MAX_SIZE; i++){
        //printf("%s %s %d\n", sorted_bigrams[i]->word1, sorted_bigrams[i]->word2, sorted_bigrams[i]->count);
    }

    // sort the array with insertion sort
    int key;
    int j;

    for(int i = 1; i < sorted_index+1; i++){
        key = sorted_bigrams[i]->count;
        j = i - 1;

        while(j >= 0 && sorted_bigrams[j]->count < key){
            sorted_bigrams[j + 1] = sorted_bigrams[j];
            j = j - 1;
        }
        sorted_bigrams[j + 1]->count = key;
    }
}

// free memory of hash table
void free_table(Node** hashtable){
    for(int i = 0; i < MAX_SIZE; i++){
        Node* temp = hashtable[i];
        while(temp != NULL){
            Node* temp2 = temp;
            temp = temp->next;
            free(temp2);
        }
    }
}

void print_hash(Node** hashtable){
    for(int i = 0; i < MAX_SIZE; i++){
        Node* temp = hashtable[i];
        while(temp != NULL){
            printf("%s %s %d\n", temp->word1, temp->word2, temp->count);
            temp = temp->next;
        }
    }

}

// main function ========================================
int main(){
    //initialize hash table
    Node** hashtable = (Node**)malloc(sizeof(Node*) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE; i++){
        hashtable[i] = NULL;
    }

    read_file_and_hash(hashtable);
    //use num_bigrams later to malloc the array
    Node** sorted_bigrams = (Node**)malloc(sizeof(Node*) * MAX_SIZE);
    for(int i = 0; i < MAX_SIZE; i++){
        sorted_bigrams[i] = NULL;
    }

    //print_hash(hashtable);
    //sort(hashtable, sorted_bigrams);

    //printf("Top bigram: %s %s", sorted_bigrams[0]->word1, sorted_bigrams[0]->word2);

    free_table(hashtable);
    
    return 0;
}