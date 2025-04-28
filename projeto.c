#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wug.h"

// Structure to track a state in our optimization search
typedef struct state {
    wug_t population[64];  
    int size;              
    int operations;
    char *history;          
} state_t;

void print_results(state_t *state, int superwug_count) {
    printf("Encontrados %d superwugs após %d operações\n", superwug_count, state->operations);
    printf("Sequência de operações: %s\n", state->history);
}

int run_simulation(const char *sequence, int target_superwugs, bool print_output) {
    wug_t population[64];
    int size = 0;
    int capacity = 64;
    int operations = 0;
    
    // Initialize with two wugs (male and female with all zeros)
    int zero_genome[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    wug_t female_wug = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, F};
    wug_t male_wug = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, M};
    
    insert_ranked(population, female_wug, &size, capacity);
    insert_ranked(population, male_wug, &size, capacity);
    
    if (print_output) {
        printf("Populacao inicial:\n");
        print_population(population, size);
    }
    
    // Run the sequence
    int superwug_count = 0;
    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == 'P') {
            if (print_output) printf("Proliferando...\n");
            proliferate(population, &size, capacity);
        } else if (sequence[i] == 'B') {
            if (print_output) printf("Reproducao...\n");
            breed(population, &size, capacity);
        }
        operations++;
        
        // Check for superwugs
        superwug_count = report_population(population, size);
        if (print_output) {
            printf("Apos a operacao %d: %d superwugs\n", operations, superwug_count);
        }
        
        if (superwug_count >= target_superwugs) {
            break;
        }
    }
    
    return operations;
}

void find_optimal_sequence(int target_superwugs) {
    printf("Encontrando a sequencia ideal para obter %d superwugs...\n", target_superwugs);
    
    // Sequences to try - combinations of breeding (B) and proliferation (P)
    const char *sequences[] = {
        "PPPPPPPPPP",       
        "BBBBBBBBBB",       
        "PBPBPBPBPB",      
        "PPBPPBPPBP",        
        "BPBPBPBPBP",         
        "PPPBPPPBPP",       
        "PBBBPBBBPB",       
        NULL
    };
    
    int best_operations = 1000;
    const char *best_sequence = NULL;
    
    for (int i = 0; sequences[i] != NULL; i++) {
        printf("Tentando sequencia: %s\n", sequences[i]);
        int ops = run_simulation(sequences[i], target_superwugs, false);
        
        if (ops < best_operations && ops > 0) {
            best_operations = ops;
            best_sequence = sequences[i];
        }
    }
    
    if (best_sequence) {
        printf("Sequencia otima encontrada: %s (primeiras %d operacoes)\n", 
               best_sequence, best_operations);
        printf("Executando sequencia otima com saida:\n");
        run_simulation(best_sequence, target_superwugs, true);
    } else {
        printf("Nenhuma sequencia bem-sucedida encontrada\n");
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL)); // Initialize random seed
    
    if (argc > 1) {
        // If a specific sequence was provided
        if (strcmp(argv[1], "-s") == 0 && argc > 2) {
            int target = (argc > 3) ? atoi(argv[3]) : 1;
            run_simulation(argv[2], target, true);
        } 
        // If we want to optimize for a certain number of superwugs
        else if (strcmp(argv[1], "-o") == 0) {
            int target = (argc > 2) ? atoi(argv[2]) : 1;
            find_optimal_sequence(target);
        }
    } else {
        // Default: find optimal sequence for one superwug
        find_optimal_sequence(1);
        
        printf("\n\nAgora tentando atingir 100%% superwugs...\n");
        find_optimal_sequence(64);  // Capacity is 64, so that would be 100%
    }
    
    return 0;
}