#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random DNA sequence
char* generateRandomSequence(int minLength, int maxLength) {
    int length = (rand() % (maxLength - minLength + 1)) + minLength;
    char* sequence = (char*)malloc(length + 1);

    for (int i = 0; i < length; i++) {
        char bases[] = "AGCT";
        sequence[i] = bases[rand() % 4];
    }
    sequence[length] = '\0';

    return sequence;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    int numSequences = (rand() % 4) + 2; // Generate 2 to 5 sequences
    //int numSequences = 3;
    const char* filename = "dna_sequences.txt";

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }
    
    fprintf(file, "%d\n", numSequences);
    fprintf(file, "$\n");

    for (int i = 0; i < numSequences; i++) {
        int minLength = 110;
        int maxLength = 120;

        char* sequence = generateRandomSequence(minLength, maxLength);
        fprintf(file, "%s\n", sequence);
        free(sequence);
    }

    fclose(file);
    printf("DNA sequences saved to %s\n", filename);

    return 0;
}
