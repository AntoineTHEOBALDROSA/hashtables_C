#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


///////////////////////////////////////////////////////////////////////////////
//////// Simple Linked lists //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct NodeLinkedList {
    int data;
    struct NodeLinkedList* next;
} NodeLinkedList;
typedef struct NodeLinkedList NodeLinkedList;


typedef struct LinkedList {
    NodeLinkedList* head;
} LinkedList;
typedef struct LinkedList LinkedList;

// create an empty LinkedList
LinkedList *LinkedList_create(){
    LinkedList *l = malloc(sizeof(LinkedList));
    l->head = NULL;
    return l;
}

bool LinkedList_is_empty(LinkedList *l){
    return (l->head == NULL);
}

// Free the memory used by a LinkedList 
void LinkedList_free(LinkedList *l){
    if (LinkedList_is_empty(l)){
        free(l);
        return;
    }
    NodeLinkedList *n = l->head;
    while (n->next != NULL){
        NodeLinkedList *temp = n->next;
        free(n);
        n = temp;
    }
    free(n);
    free(l);
}

// Return true iff x is in the LinkedList l
bool LinkedList_mem(LinkedList *l, int x){
    if (LinkedList_is_empty(l)) return false;
    NodeLinkedList *n = l->head;
    while (n->next != NULL){
        if (n->data == x) return true;
        else n = n->next;
    }
    return n->data == x;
}

void LinkedList_push(LinkedList *l, int x){
    NodeLinkedList *n = malloc(sizeof(NodeLinkedList));
    n->data = x;
    n->next = l->head;
    l->head = n;
}

int LinkedList_pop(LinkedList *l){
    assert(LinkedList_is_empty(l) == false);
    NodeLinkedList *temp = l->head;
    int val_temp = temp->data;
    l->head = temp->next;
    free(temp);
    return val_temp;
}

void LinkedList_delete(LinkedList *l, int x){
    if (LinkedList_is_empty(l)) return;
    NodeLinkedList *n = l->head;
    if (n->next == NULL){
        if(n->data == x) {
            // there is only one element and it's the one we want to delete
            l->head = NULL;
        } 
        return;
    }
    NodeLinkedList *n_prev = l->head;
    n = n->next;
    if(n_prev->data == x){
        l->head = n;
        return;
    }
    while (n->next != NULL){
        if (n->data == x) {
            n_prev->next = n->next;
            free(n);
            return;
        }
        else {
            n = n->next;
            n_prev = n_prev->next;
        }
    }
    if(n->data == x) {
        // the element we want to delete is the last of the linked list
        n_prev->next = NULL;
    } 
    return;
}

void LinkedList_printf(LinkedList *l){
    NodeLinkedList *n = l->head;
    printf("[");
    while(n != NULL){
        printf("%d, ", n->data);
        n = n->next;
    }
    printf("]\n");
}


///////////////////////////////////////////////////////////////////////////////
//////// Hashtable  ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Implementation :
// Chaining (we use buckets represented as linked lists for collisions
// The hash functions will be universal hashing to garantee Adding in an average of O(1) 

int a, b, m;
const int p = 15733; // p is a prime greater than m

// Define the structure for the hash table
typedef struct HashTable {
    LinkedList** buckets;
} HashTable;

// MUST BE EXECUTED BEFORE ANY OTHER HashTable FUNCTIONS 
// Initialize hash parameters
// size represents the number of buckets in the hashmap
// Such a function is useful only if the hashmap functions are in another files
void initialize_hash_parameters(int size){
    if (size > p){
        printf("Warning! The hashmap is too big and operations may not execute in O(1).\n You should change the const p to a bigger prime.");
    }
    
    a = 1 + rand() % (p - 1); // 1 <= a <= p-1
    b = rand() % (p - 1); // 0 <= b =< p -1
    m = size;
}

// Create hash table
// Size is set into the initialize_hash_parameters function
HashTable* create_hash_table(){
    LinkedList **buck = malloc(sizeof(LinkedList) * m);
    for(int i = 0; i < m; i++){
        buck[i] = LinkedList_create();
    }

    HashTable* h = malloc(sizeof(HashTable));
    h->buckets = buck;
    return h;
}

// Universal hash function
int universal_hash_function(int key) {
    return ((((a * key + b) % p) + p) %p ) % m;
}

// Function to insert a key-value pair into the hash table
void hash_table_insert(HashTable* h, int value){
    int k = universal_hash_function(value);
    LinkedList_push(h->buckets[k], value);
}

// Function to search for a value by key in the hash table
bool hash_table_search(HashTable* h, int value){
    int k = universal_hash_function(value);
    return LinkedList_mem(h->buckets[k], value);
}

// Function to remove a key-value pair from the hash table
void hash_table_remove(HashTable* h, int value){
    int k = universal_hash_function(value);
    LinkedList_delete(h->buckets[k], value);
}

// Function to free the hash table
void free_hash_table(HashTable* h){
    for(int i = 0; i < m; i++){
        LinkedList_free(h->buckets[i]);
    }
    free(h->buckets);
    free(h);
}


int main(){

    int m = 100; // Size of the hashtable
    initialize_hash_parameters(m);

    HashTable *h = create_hash_table();
    
    hash_table_insert(h, 1);
    printf("Inserting 1 in the hashtable.\n");
    hash_table_insert(h, 10);
    printf("Inserting 10 in the hashtable.\n");
    hash_table_insert(h, 20);
    printf("Inserting 20 in the hashtable.\n");

    for(int i = 0; i < 21; i++){
        printf("%d was found in h : %d\n", i, hash_table_search(h, i));
    }

    hash_table_remove(h, 10);
    printf("Removing 10 from the hashtable.\n");

    printf("%d was found in h : %d\n", 10, hash_table_search(h, 10));

    return 0;
}
   