#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 150
#define MAX_SEQUENCES 5

//struct definitions ===========================================================
typedef struct dna_seq{
    char* sequence;
    int length;
    int** letter; // stores the leters of each letter's indexes (0 = A, 1 = T, 2 = G, 3 = C)
} DNA_SEQ;

typedef struct node{
    int* str_indexes[2]; //stores indexes of each letter for each sequence and its next one
    int lcs_length;
    struct node* next_node;
}NODE;

// helper functions ============================================================
// takes input
void take_input(DNA_SEQ* dna, int* string_num){
    FILE *input_file = fopen("hw2_input.txt", "r");

    fscanf(input_file, "%d", string_num); //scan number of inputs
    fscanf(input_file, "%*s"); //remove unnecessary characters

    //scan the dna subsequences
    for(int i = 0; i < *string_num; i++){
        dna[i].sequence = malloc(sizeof(char) * MAX_LENGTH);
        fscanf(input_file, "%s", dna[i].sequence);
        dna[i].length = strlen(dna[i].sequence);

        dna[i].letter = malloc(sizeof(int*) * 4); // 4 letters [A, T, G, C]
        for(int j = 0; j < 4; j++){
            //dna[i].letter[j] = malloc(sizeof(int) * dna[i].length);
            dna[i].letter[j] = malloc(sizeof(int) * MAX_LENGTH);
            for(int k = 0; k < MAX_LENGTH; k++) dna[i].letter[j][k] = -1;
        }
        // fill in the letter array with indexes of each character
        int a_val = -1;
        int t_val = -1;
        int g_val = -1;
        int c_val = -1;
        for(int j = dna[i].length-1; j >= 0; j--){
            if(dna[i].sequence[j] == 'A'){
                dna[i].letter[0][j] = j+1;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = c_val;
                a_val = j+1;
            }else if(dna[i].sequence[j] == 'T'){
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = j+1;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = c_val;
                t_val = j+1;
            }else if(dna[i].sequence[j] == 'G'){
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = j+1;
                dna[i].letter[3][j] = c_val;
                g_val = j+1;
            }else{
                dna[i].letter[0][j] = a_val;
                dna[i].letter[1][j] = t_val;
                dna[i].letter[2][j] = g_val;
                dna[i].letter[3][j] = j+1;
                c_val = j+1;
            }
        }
    }

    fclose(input_file);
}


// returns 1 if the values are the same at the given index
int is_common(DNA_SEQ* dna, int size, char* lcs, int lcs_index, int indexes[5]){
    for(int i = 0; i < size; i++){
        if(dna[i].sequence[indexes[i]] != lcs[lcs_index]){
            return 0;
        }
    }
    return 1;
}

// write final inputs to file (aligned dna sequences and asterisks to mark lcs)
void write_final_results(DNA_SEQ* dna, int size, char* lcs){
    int lcs_index = 0;
    char* aligned_dna[size];
    int max_length = 0;
    int indexes[5] = {0};

    // fill with dashes
    for(int i = 0; i < size; i++){
        aligned_dna[i] = malloc(sizeof(char) * 500);
        for (int j = 0; j < 500; j++) {
            aligned_dna[i][j] = '-';
        }
    }
    
    FILE *output;
    output = fopen("hw2_output.txt", "w");

    int aligned_index = 0;
    int loop = 1;
    while(loop){
        for(int i = 0; i < size; i++){
            if(dna[i].sequence[indexes[i]] != lcs[lcs_index]){
                aligned_dna[i][aligned_index] = dna[i].sequence[indexes[i]];
            }else if (lcs[lcs_index] == dna[i].sequence[indexes[i]] && is_common(dna, size, lcs, lcs_index, indexes)){ //aligned
                for(int j = 0; j < size; j++) aligned_dna[j][aligned_index] = dna[j].sequence[indexes[j]];
                lcs_index++;
                break;
            }else{
                indexes[i]--; //if not aligned, leave as dash
            } 
        }
        aligned_index++;
        for(int i = 0; i <size; i++) indexes[i]++; //increment indexes
        
        //check if indexes are within bound
        for(int i = 0; i < size; i++){
            if(indexes[i] >= dna[i].length) {
                loop = 0;
                break;
            }
        }
    }

    //appending any left over characters after alignment
    int longest_aligned_length = 0;
    for(int i = 0; i < size; i++){
        if(indexes[i] < dna[i].length){
            int temp_aligned_index = aligned_index;
            if((dna[i].length - indexes[i]) > longest_aligned_length) longest_aligned_length = dna[i].length - indexes[i];
            for(int j = indexes[i]; j < dna[i].length; j++){
                aligned_dna[i][temp_aligned_index] = dna[i].sequence[j];
                temp_aligned_index++;
            }
        }
    }
    longest_aligned_length += aligned_index;

    // write aligned sequences to file
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < longest_aligned_length; j++) fputc(aligned_dna[i][j], output);
        fputc('\n', output);
    }

    // print asterisks 
    lcs_index = 0;
    for(int j = 0; j < aligned_index; j++){
        if(lcs_index > (strlen(lcs))) break;
        else if(lcs[lcs_index] == aligned_dna[0][j]){
            fputc('*', output);
            lcs_index++;
        }else{
            fputc(' ', output);
        }
    }
    
    for(int i = 0; i < size; i ++) free(aligned_dna[i]);
    fclose(output);
}

// lcs functions ================================================================

char* find_lcs2(DNA_SEQ* dna, int string_num){
    // create table
    int** table = (int**)malloc(sizeof(int*) * (dna[0].length+1));
    for(int i = 0; i < dna[0].length+1; i++){
        table[i] = (int*)malloc(sizeof(int) * (dna[1].length+1));
    }

    // fill in table
    for(int i = 0; i < dna[0].length+1; i++){
        for(int j = 0; j < dna[1].length+1; j++){
            if(i == 0 || j == 0){
                table[i][j] = 0;
            }else if(dna[0].sequence[i-1] == dna[1].sequence[j-1]){
                table[i][j] = table[i-1][j-1] + 1;
            }else{
                //take the maximum of either the top or the left
                if(table[i-1][j] > table[i][j-1]){
                    table[i][j] = table[i-1][j];
                }else{
                    table[i][j] = table[i][j-1];
                }
            }
        }
    }

    // create lcs
    int lcs_length = table[dna[0].length][dna[1].length];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int lcs_index = lcs_length-1;
    int i = dna[0].length; int j = dna[1].length;
    while(i > 0 && j > 0){
        if(table[i-1][j] == table[i][j]){
            i--;
        }else if(table[i][j-1] == table[i][j]){
            j--;
        }else{
            lcs[lcs_index] = dna[0].sequence[i-1];
            i--; j--;
            lcs_index--;
        }
    }

    free(table);

    return lcs;
}

char* find_lcs3(DNA_SEQ* dna, int string_num){
    // create table
    int*** table = (int ***)malloc(sizeof(int**) * (dna[0].length+1));
    
    for(int i = 0; i < dna[0].length+1; i++){
        table[i] = (int**)malloc(sizeof(int*) * (dna[1].length+1));
        for(int j = 0; j < dna[1].length+1; j++){
            table[i][j] = (int*)malloc(sizeof(int) * (dna[2].length+1));
        }
    }

    // fill in table
    for(int i = 0; i < dna[0].length+1; i ++){
        for(int j = 0; j < dna[1].length+1; j++){
            for(int k = 0; k < dna[2].length+1; k++){   
                if(i == 0 || j == 0 || k == 0){
                    table[i][j][k] = 0;
                }else if (dna[0].sequence[i-1] == dna[1].sequence[j-1] && dna[1].sequence[j-1] == dna[2].sequence[k-1]){
                    table[i][j][k] = table[i-1][j-1][k-1] + 1;
                }else{
                    int max = table[i-1][j][k];
                    if(table[i][j-1][k] > max) max = table[i][j-1][k];
                    if(table[i][j][k-1] > max) max = table[i][j][k-1];
                    table[i][j][k] = max;
                }
            }
        }
    }

    // create lcs
    int lcs_length = table[dna[0].length][dna[1].length][dna[2].length];
    char* lcs = malloc(sizeof(char) * lcs_length);
    lcs[lcs_length] = '\0';

    // backtracking to find lcs
    int i = dna[0].length; int j = dna[1].length; int k = dna[2].length;
    int lcs_index = lcs_length-1;
    while(i > 0 && j > 0 && k > 0){
        if(table[i-1][j][k] == table[i][j][k]){
            i--;
        }else if(table[i][j-1][k] == table[i][j][k]){
            j--;
        }else if(table[i][j][k-1] == table[i][j][k]){
            k--;
        }else{
            lcs[lcs_index] = dna[0].sequence[i-1];
            i--; j--; k--;
            lcs_index--;
        }
    }

    free(table);

    return lcs;
}

int calc_lcs_length4(DNA_SEQ* dna, int string_num, NODE***** table, int* indexes){
    // if the indexes equal to -1, it indicated the end of string and hence, the length is 0
    for(int i = 0; i < string_num; i++){
        if(indexes[i] == -1) return 0;
    }

    // if indexes are valid, check if the value is already in the table
    NODE* current_node = table[indexes[0]][indexes[1]][indexes[2]][indexes[3]];

    // if the value is already in the table, then return its length
    if(current_node != NULL){
        return current_node->lcs_length;
    }
    //otherwise append that node to the memoization table
    else{
        NODE *new_node = (NODE*)malloc(sizeof(NODE));
        table[indexes[0]][indexes[1]][indexes[2]][indexes[3]] = new_node;

        //initialize the str_indexes
        for(int i = 0; i < 2; i++){
            new_node->str_indexes[i] = malloc(sizeof(int) * string_num);
        }

        //save the indexes as the new node's str_indexes
        for(int i = 0; i < string_num; i++){
            new_node->str_indexes[0][i] = indexes[i];
        }

        //calculate the lcs length by looking at the next four nodes
        int next_four_indexes[4][string_num];
        for(int i = 0; i < string_num; i++){
            for(int j = 0; j < 4; j++){
                next_four_indexes[j][i] = dna[i].letter[j][indexes[i]];
            }
        }

        int next_four_node_lengths[4];
        for(int i = 0; i < 4; i++){
            next_four_node_lengths[i] = calc_lcs_length4(dna, string_num, table, next_four_indexes[i]);
        }

        //find the max length
        int max_length = next_four_node_lengths[0];
        int max_index = 0;
        for(int i = 1; i < 4; i++){
            if(next_four_node_lengths[i] > max_length) {
                max_length = next_four_node_lengths[i];
                max_index = i;
            }
        }

        new_node->lcs_length = max_length+1; 
        //save the next values into str_indexes
        for(int i = 0; i < string_num; i++){
            new_node->str_indexes[1][i] = next_four_indexes[max_index][i]; 
        }

        return max_length+1; //+1 becaue the current node is also included
    }
}

char* traceback4(DNA_SEQ* dna, int string_num, int lcs_length, NODE***** table){
    char* lcs = malloc(sizeof(char) * (lcs_length+1));
    lcs[lcs_length] = '\0';

    NODE* current_node = table[0][0][0][0]; //start at the root 

    for(int i = 0; i <lcs_length; i++){
        NODE* next_node = table[current_node->str_indexes[1][0]][current_node->str_indexes[1][1]][current_node->str_indexes[1][2]][current_node->str_indexes[1][3]];
        
        //find the next node
        if(next_node == NULL) break;
        else current_node = next_node;

        lcs[i] = dna[0].sequence[current_node->str_indexes[0][0]-1];
    }

    return lcs; 
}

char* find_lcs4(DNA_SEQ* dna, int string_num){
    // creating memoization table (4D array)
    NODE***** table = (NODE*****)malloc(sizeof(NODE****) * MAX_LENGTH);

    for(int i = 0; i < MAX_LENGTH; i++){
        table[i] = (NODE****)malloc(sizeof(NODE***) * MAX_LENGTH);
        for(int j = 0; j < MAX_LENGTH; j++){
            table[i][j] = (NODE***)malloc(sizeof(NODE**) * MAX_LENGTH);
            for(int k = 0; k < MAX_LENGTH; k++){
                table[i][j][k] = (NODE**)malloc(sizeof(NODE*) * MAX_LENGTH);
            }
        }
    }
    //starting index is all 0's
    int* starting_indexes = malloc(sizeof(int) * string_num);
    for(int i = 0; i < string_num; i++) starting_indexes[i] = 0;
    // calculate length of lcs
    int lcs_length = calc_lcs_length4(dna, string_num, table, starting_indexes)-1;
    //find lcs
    char* lcs = traceback4(dna, string_num, lcs_length, table);

    return lcs;
}

int node_matches_table_val(int indexes[5], NODE***** table, NODE** current_node){
    while ((*current_node) != NULL){
        //the linked node's 5th index and indexes[4] are the same, return 1
        if((*current_node)->str_indexes[0][4] == indexes[4]){
            return 1;
        }
        //if the next node doesn't exist, then return 0
        if((*current_node)->next_node == NULL){
            return 0;
        }
        (*current_node) = (*current_node)->next_node;
    }
    return 0;
}

int calc_lcs_length5(DNA_SEQ* dna, int string_num, NODE***** table, int* indexes){
    // if the indexes equal to -1, it indicated the end of string and hence, the length is 0
    for(int i = 0; i < string_num; i++){
        if(indexes[i] == -1) return 0;
    }

    // if indexes are valid, check if the value is already in the table
    NODE* current_node = table[indexes[0]][indexes[1]][indexes[2]][indexes[3]];

    // if the value is already in the table, then return its length
    if(node_matches_table_val(indexes, table, &current_node)){
        return current_node->lcs_length;
    }
    // //otherwise append that node to the memoization table
    else{
        NODE *new_node = (NODE*)malloc(sizeof(NODE));
        
        // append new node to the table or linked list
        if(current_node == NULL){
            table[indexes[0]][indexes[1]][indexes[2]][indexes[3]] = new_node;
        }else{
            current_node->next_node = new_node;
        }
        
        //initialize the str_indexes
        for(int i = 0; i < 2; i++){
            new_node->str_indexes[i] = malloc(sizeof(int) * string_num);
        }

        //save the indexes as the new node's str_indexes
        for(int i = 0; i < string_num; i++){
            new_node->str_indexes[0][i] = indexes[i];
        }

        //calculate the lcs length by looking at the next four nodes
        int next_four_indexes[4][string_num];
        for(int i = 0; i < string_num; i++){
            for(int j = 0; j < 4; j++){
                next_four_indexes[j][i] = dna[i].letter[j][indexes[i]];
            }
        }

        int next_four_node_lengths[4];
        for(int i = 0; i < 4; i++){
            next_four_node_lengths[i] = calc_lcs_length5(dna, string_num, table, next_four_indexes[i]);
        }

        //find the max length
        int max_length = next_four_node_lengths[0];
        int max_index = 0;
        for(int i = 1; i < 4; i++){
            if(next_four_node_lengths[i] > max_length) {
                max_length = next_four_node_lengths[i];
                max_index = i;
            }
        }

        new_node->lcs_length = max_length+1; 
        //save the next values into str_indexes
        for(int i = 0; i < string_num; i++){
            new_node->str_indexes[1][i] = next_four_indexes[max_index][i]; 
        }

        return max_length+1; //+1 becaue the current node is also included
    }
}

char* traceback5(DNA_SEQ* dna, int string_num, int lcs_length, NODE***** table){
    char* lcs = malloc(sizeof(char) * (lcs_length+1));
    lcs[lcs_length] = '\0';

    NODE* current_node = table[0][0][0][0]; //start at the root 

    for(int i = 0; i <lcs_length; i++){
        NODE* after_node = table[current_node->str_indexes[1][0]][current_node->str_indexes[1][1]][current_node->str_indexes[1][2]][current_node->str_indexes[1][3]];

        if(!node_matches_table_val(current_node->str_indexes[1], table, &after_node)){
            break;
        }else{
            current_node = after_node;
        }
        
        lcs[i] = dna[0].sequence[current_node->str_indexes[0][0]-1];
        current_node = after_node;
    }
    return lcs; 
}

char* find_lcs5(DNA_SEQ* dna, int string_num){
    NODE***** table = (NODE*****)malloc(sizeof(NODE****) * MAX_LENGTH);

    for(int i = 0; i < MAX_LENGTH; i++){
        table[i] = (NODE****)malloc(sizeof(NODE***) * MAX_LENGTH);
        for(int j = 0; j < MAX_LENGTH; j++){
            table[i][j] = (NODE***)malloc(sizeof(NODE**) * MAX_LENGTH);
            for(int k = 0; k < MAX_LENGTH; k++){
                table[i][j][k] = (NODE**)malloc(sizeof(NODE*) * MAX_LENGTH);
            }
        }
    }

    //starting index is all 0's
    int* starting_indexes = malloc(sizeof(int) * string_num);
    for(int i = 0; i < string_num; i++) starting_indexes[i] = 0;

    // calculate length of lcs
    int lcs_length = calc_lcs_length5(dna, string_num, table, starting_indexes)-1;
    //find lcs
    char* lcs = traceback5(dna, string_num, lcs_length, table);
    //char* lcs = "ATCCAT";

    return lcs;
}

// main function ================================================================
int main(){
    // Checking run time
    // int TIME = 0;
    // clock_t start = clock();

    int string_num = 0;
    DNA_SEQ dna[MAX_SEQUENCES];
    take_input(dna, &string_num); //store the input sequences

    char* lcs;
    if(string_num == 2){
        lcs = find_lcs2(dna, string_num);
    }else if(string_num == 3){
        lcs = find_lcs3(dna, string_num);
    }else if(string_num == 4){
        lcs = find_lcs4(dna, string_num);
    }else{
        lcs = find_lcs5(dna, string_num);
    }
    //write final output
    write_final_results(dna, string_num, lcs);

    //free memory
    for(int i = 0; i < string_num; i++) free(dna[i].sequence);

    // TIME += ((int)clock() - start) / (CLOCKS_PER_SEC / 1000);
    // printf("TIME : %d ms\n", TIME);
    return 0;
}