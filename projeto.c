#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wug.h"

// Structure to track a state in our optimization search
typedef struct state {
    wug_t population[64];  // Population of wugs
    int size;              // Current population size
    int operations;        // Number of operations performed to reach this state
    char *history;         // History of operations (P for proliferate, B for breed)
} state_t;

// Print the results of our optimization
void print_results(state_t *state, int superwug_count) {
    printf("Found %d superwugs after %d operations\n", superwug_count, state->operations);
    printf("Operation sequence: %s\n", state->history);
}

// Run the simulation with a specific breeding/proliferation sequence
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
        printf("Initial population:\n");
        print_population(population, size);
    }
    
    // Run the sequence
    int superwug_count = 0;
    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == 'P') {
            if (print_output) printf("Proliferating...\n");
            proliferate(population, &size, capacity);
        } else if (sequence[i] == 'B') {
            if (print_output) printf("Breeding...\n");
            breed(population, &size, capacity);
        }
        operations++;
        
        // Check for superwugs
        superwug_count = report_population(population, size);
        if (print_output) {
            printf("After operation %d: %d superwugs\n", operations, superwug_count);
        }
        
        // If we've reached the target
        if (superwug_count >= target_superwugs) {
            break;
        }
    }
    
    return operations;
}

// Try different breeding/proliferation sequences to find optimal
void find_optimal_sequence(int target_superwugs) {
    printf("Finding optimal sequence to get %d superwugs...\n", target_superwugs);
    
    // Sequences to try - combinations of breeding (B) and proliferation (P)
    const char *sequences[] = {
        "PPPPPPPPPP",      // All proliferation
        "BBBBBBBBBB",      // All breeding
        "PBPBPBPBPB",      // Alternating
        "PPBPPBPPBP",      // 2 proliferate, 1 breed
        "BPBPBPBPBP",      // Alternating starting with breeding
        "PPPBPPPBPP",      // 3 proliferate, 1 breed
        "PBBBPBBBPB",      // 1 proliferate, 3 breed
        NULL
    };
    
    int best_operations = 1000;
    const char *best_sequence = NULL;
    
    for (int i = 0; sequences[i] != NULL; i++) {
        printf("Trying sequence: %s\n", sequences[i]);
        int ops = run_simulation(sequences[i], target_superwugs, false);
        
        if (ops < best_operations && ops > 0) {
            best_operations = ops;
            best_sequence = sequences[i];
        }
    }
    
    if (best_sequence) {
        printf("Optimal sequence found: %s (first %d operations)\n", 
               best_sequence, best_operations);
        printf("Running optimal sequence with output:\n");
        run_simulation(best_sequence, target_superwugs, true);
    } else {
        printf("No successful sequence found\n");
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
        
        printf("\n\nNow trying to reach 100%% superwugs...\n");
        find_optimal_sequence(64);  // Capacity is 64, so that would be 100%
    }
    
    return 0;
}