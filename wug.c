#include "wug.h"

// Superwug genome and characteristic zones
const int SUPERWUG_GENOME[16] = {1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0};

// Zone indices for each characteristic
// Intelligence: positions 0, 2, 7, 13
// Beauty: positions 1, 2, 3, 12
// Strength: positions 8, 9, 10, 11
// Speed: positions 4, 6, 14, 15
const int INTELLIGENCE_ZONE[4] = {0, 2, 7, 13};
const int BEAUTY_ZONE[4] = {1, 2, 3, 12};
const int STRENGTH_ZONE[4] = {8, 9, 10, 11};
const int SPEED_ZONE[4] = {4, 6, 14, 15};

// Function to create a new wug
wug_t* create_wug(int genome[16], gender_t g) {
    wug_t* new_wug = (wug_t*)malloc(sizeof(wug_t));
    if (new_wug == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < 16; i++) {
        new_wug->genome[i] = genome[i];
    }
    new_wug->gender = g;
    
    return new_wug;
}

// Function to print a wug
void print_wug(const wug_t *a) {
    int features[4];
    genome2features(a->genome, features);
    printf("%s %c (%s)\n", array_string(a->genome, 16), a->gender, array_string(features, 4));
}

// Function to print the population
void print_population(const wug_t population[], const int size) {
    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        print_wug(&population[i]);
    }
}

// Question 1: Decode genome to features
void genome2features(const int genome[16], int features[4]) {
    // Check intelligence (zone 0)
    features[0] = 1; // Start with assumption it's exceptional
    for (int i = 0; i < 4; i++) {
        if (genome[INTELLIGENCE_ZONE[i]] != SUPERWUG_GENOME[INTELLIGENCE_ZONE[i]]) {
            features[0] = 0; // Not exceptional
            break;
        }
    }
    
    // Check beauty (zone 1)
    features[1] = 1; // Start with assumption it's exceptional
    for (int i = 0; i < 4; i++) {
        if (genome[BEAUTY_ZONE[i]] != SUPERWUG_GENOME[BEAUTY_ZONE[i]]) {
            features[1] = 0; // Not exceptional
            break;
        }
    }
    
    // Check strength (zone 2)
    features[2] = 1; // Start with assumption it's exceptional
    for (int i = 0; i < 4; i++) {
        if (genome[STRENGTH_ZONE[i]] != SUPERWUG_GENOME[STRENGTH_ZONE[i]]) {
            features[2] = 0; // Not exceptional
            break;
        }
    }
    
    // Check speed (zone 3)
    features[3] = 1; // Start with assumption it's exceptional
    for (int i = 0; i < 4; i++) {
        if (genome[SPEED_ZONE[i]] != SUPERWUG_GENOME[SPEED_ZONE[i]]) {
            features[3] = 0; // Not exceptional
            break;
        }
    }
}

// Question 2: Report population statistics
int report_population(const wug_t population[], const int size) {
    int counts[16][2] = {0}; // [feature pattern][gender]
    int superwug_count = 0;
    
    for (int i = 0; i < size; i++) {
        int features[4];
        genome2features(population[i].genome, features);
        uint8_t feature_bits = features2bits(features);
        
        // Count by feature pattern and gender
        if (population[i].gender == M) {
            counts[feature_bits][0]++;
        } else {
            counts[feature_bits][1]++;
        }
        
        // Count superwugs (all features are exceptional)
        if (feature_bits == 15) { // 1111 in binary
            superwug_count++;
        }
    }
    
    // Print the population summary
    printf("Population Summary:\n");
    for (int i = 0; i < 16; i++) {
        if (counts[i][0] > 0 || counts[i][1] > 0) {
            int features[4];
            bits2features(i, features);
            printf("Features %s: %d M, %d F\n", array_string(features, 4), counts[i][0], counts[i][1]);
        }
    }
    
    return superwug_count;
}

// Question 3: Calculate wug rank
int rank(const wug_t i) {
    int features[4];
    genome2features(i.genome, features);
    
    int rank_value = 0;
    for (int j = 0; j < 4; j++) {
        if (features[j] == 1) {
            rank_value++;
        }
    }
    
    return rank_value;
}

// Question 4: Insert wug into population maintaining rank order
bool insert_ranked(wug_t population[], wug_t e, int *size, int capacity) {
    // If population is at capacity, check if new wug has higher rank than lowest ranked wug
    if (*size == capacity) {
        int lowest_rank = rank(population[*size - 1]);
        if (rank(e) <= lowest_rank) {
            return false; // New wug not added
        }
    }
    
    // Find position to insert the new wug
    int i;
    int new_rank = rank(e);
    for (i = 0; i < *size; i++) {
        if (new_rank > rank(population[i])) {
            break;
        }
    }
    
    // If adding at the end and we're at capacity, don't add
    if (i == *size && *size == capacity) {
        return false;
    }
    
    // Shift elements to make space
    if (i < *size) {
        for (int j = (*size < capacity ? *size : *size - 1); j > i; j--) {
            population[j] = population[j - 1];
        }
    }
    
    // Insert the new wug
    population[i] = e;
    
    // Update size if not at capacity
    if (*size < capacity) {
        (*size)++;
    }
    
    return true;
}

// Question 5: Proliferate wugs through cloning with mutation
void proliferate(wug_t population[], int *size, int capacity) {
    // Create a linked list to store clones
    node_t *clones_head = NULL;
    
    // Get the current population size (since it will change)
    int current_size = *size;
    
    // For each wug in the current population
    for (int i = 0; i < current_size; i++) {
        // Create 16 clones with one gene changed
        for (int gene_pos = 0; gene_pos < 16; gene_pos++) {
            // Create a clone with one gene flipped
            wug_t clone = population[i];
            clone.genome[gene_pos] = 1 - clone.genome[gene_pos]; // Flip 0->1 or 1->0
            
            // Add to the clones list
            wug_t *new_wug = create_wug(clone.genome, clone.gender);
            clones_head = list_insert(new_wug, clones_head);
        }
    }
    
    // Now insert all clones into the population
    node_t *current = clones_head;
    while (current != NULL) {
        insert_ranked(population, *(current->wug), size, capacity);
        current = current->next;
    }
    
    // Free the list (but not the wugs as they might be in the population)
    list_free(clones_head, false);
}

// Calculate match score between two wugs (for breeding)
int match(const wug_t *a, const wug_t *b) {
    // Check if they are of opposite gender
    if (a->gender == b->gender) {
        return -1; // Not a valid match
    }
    
    int a_features[4], b_features[4];
    genome2features(a->genome, a_features);
    genome2features(b->genome, b_features);
    
    // Count matching features
    int match_count = 0;
    for (int i = 0; i < 4; i++) {
        if (a_features[i] == b_features[i]) {
            match_count++;
        }
    }
    
    return match_count;
}

// Find the best pair of wugs for breeding
void find_pair(node_t **head, wug_t **male, wug_t **female) {
    node_t *best_male = NULL;
    node_t *best_female = NULL;
    node_t *current = *head;
    node_t *prev = NULL;
    int best_match = -1;
    
    // Find the first male and female
    while (current != NULL) {
        if (current->wug->gender == M && best_male == NULL) {
            best_male = current;
        } else if (current->wug->gender == F && best_female == NULL) {
            best_female = current;
        }
        
        if (best_male != NULL && best_female != NULL) {
            break;
        }
        
        prev = current;
        current = current->next;
    }
    
    // If we don't have a pair, return NULL
    if (best_male == NULL || best_female == NULL) {
        *male = NULL;
        *female = NULL;
        return;
    }
    
    best_match = match(best_male->wug, best_female->wug);
    
    // Find the best matching pair
    current = *head;
    prev = NULL;
    
    while (current != NULL) {
        node_t *next = current->next;
        node_t *test_node = (current->wug->gender == M) ? best_female : best_male;
        
        if (test_node != NULL) {
            int current_match = match(current->wug, test_node->wug);
            
            if (current_match > best_match) {
                if (current->wug->gender == M) {
                    best_male = current;
                } else {
                    best_female = current;
                }
                best_match = current_match;
            }
        }
        
        prev = current;
        current = next;
    }
    
    // Remove the pair from the list
    *male = best_male->wug;
    *female = best_female->wug;
    
    // Remove the nodes from the list
    node_t *temp = *head;
    node_t *prev_node = NULL;
    
    // Remove male
    while (temp != NULL) {
        if (temp == best_male) {
            if (prev_node == NULL) {
                *head = temp->next;
            } else {
                prev_node->next = temp->next;
            }
            free(temp);
            break;
        }
        prev_node = temp;
        temp = temp->next;
    }
    
    // Remove female
    temp = *head;
    prev_node = NULL;
    while (temp != NULL) {
        if (temp == best_female) {
            if (prev_node == NULL) {
                *head = temp->next;
            } else {
                prev_node->next = temp->next;
            }
            free(temp);
            break;
        }
        prev_node = temp;
        temp = temp->next;
    }
}

// Create offspring from a male and female wug pair
void create_offsprings(wug_t fem, wug_t masc, node_t **clones) {
    // Create basic genome (first half from female, second half from male)
    int basic_genome[16];
    for (int i = 0; i < 8; i++) {
        basic_genome[i] = fem.genome[i];
    }
    for (int i = 8; i < 16; i++) {
        basic_genome[i] = masc.genome[i];
    }
    
    // Create 16 mutated offspring
    for (int gene_pos = 0; gene_pos < 16; gene_pos++) {
        int mutated_genome[16];
        for (int i = 0; i < 16; i++) {
            mutated_genome[i] = basic_genome[i];
        }
        
        // Flip one gene
        mutated_genome[gene_pos] = 1 - mutated_genome[gene_pos];
        
        // Determine appropriate gender based on mutation position
        gender_t offspring_gender;
        
        // If mutation is in female part, make male to propagate mutation
        // If mutation is in male part, make female to propagate mutation
        offspring_gender = (gene_pos < 8) ? M : F;
        
        // Create the new wug and add to list
        wug_t *new_wug = create_wug(mutated_genome, offspring_gender);
        *clones = list_insert(new_wug, *clones);
    }
    
    // Create two additional offspring with the basic genome (one male, one female)
    wug_t *male_offspring = create_wug(basic_genome, M);
    wug_t *female_offspring = create_wug(basic_genome, F);
    
    *clones = list_insert(male_offspring, *clones);
    *clones = list_insert(female_offspring, *clones);
}

// Question 6: Breeding function
void breed(wug_t population[], int *size, int capacity) {
    // Create a list of all wugs in the population
    node_t *wug_list = NULL;
    for (int i = 0; i < *size; i++) {
        wug_t *wug_copy = create_wug(population[i].genome, population[i].gender);
        wug_list = list_insert(wug_copy, wug_list);
    }
    
    // Reverse the list to get wugs in order of rank (highest first)
    wug_list = list_reverse(wug_list);
    
    // Create a list for all offspring
    node_t *offspring_list = NULL;
    
    // While we can find pairs to breed
    wug_t *male, *female;
    find_pair(&wug_list, &male, &female);
    
    while (male != NULL && female != NULL) {
        // Create offspring from this pair
        create_offsprings(*female, *male, &offspring_list);
        
        // Find next pair
        find_pair(&wug_list, &male, &female);
    }
    
    // Free the remaining wug list (and the wugs since they're copies)
    list_free(wug_list, true);
    
    // Insert all offspring into the population
    node_t *current = offspring_list;
    while (current != NULL) {
        insert_ranked(population, *(current->wug), size, capacity);
        current = current->next;
    }
    
    // Free the offspring list (but not the wugs as they might be in the population)
    list_free(offspring_list, false);
}