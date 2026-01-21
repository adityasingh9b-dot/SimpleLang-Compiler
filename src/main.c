#include <stdio.h>
#include "../include/lexer.h"
#include "../include/parser.h"

int main() {
   
   // main function yaha se start ho raha hai, be careful ...........................
   
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Could not open input.txt");
        return 1;
    }

    printf("--- Starting Compilation ---\n");

    
    parse_program(file);

    printf("\n--- Symbol Table Verification ---\n");
    

    fclose(file);
    printf("\nCompilation Finished.\n");
    
    return 0;
}
