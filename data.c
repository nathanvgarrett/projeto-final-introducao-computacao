#include "data.h"
#include <stdlib.h>

// Insert a new node to the front of the linked list
node_t* list_insert(wug_t *e, node_t *head) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return head; // Failed to allocate memory
    }
    
    new_node->wug = e;
    new_node->next = head;
    
    return new_node;
}

// Reverse a linked list
node_t* list_reverse(node_t *head) {
    node_t *prev = NULL;
    node_t *current = head;
    node_t *next = NULL;
    
    while (current != NULL) {
        next = current->next;  // Store next node
        current->next = prev;  // Reverse the link
        
        // Move one step forward
        prev = current;
        current = next;
    }
    
    // 'prev' now points to the new head
    return prev;
}

// Free the memory used by a linked list
void list_free(node_t *head, bool deep) {
    node_t *current = head;
    node_t *next;
    
    while (current != NULL) {
        next = current->next;
        
        if (deep && current->wug != NULL) {
            free(current->wug);
        }
        
        free(current);
        current = next;
    }
}

// Calculate the size of a linked list
int list_size(node_t *head) {
    int count = 0;
    node_t *current = head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count;
}