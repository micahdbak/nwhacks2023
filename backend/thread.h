#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "thread.h"

#define FILENAME "database.txt"

enum thread_type {Comment, Post, Folder};

// Forward declaration
struct Thread;
typedef struct Thread thread;

// Node structure that represents a single element inside linked list
struct Node {
    thread* thr;
    struct Node* next;
};
typedef struct Node node_t;

node_t* create_node (thread* _thr, node_t* _next);
void delete_node (node_t** node);


// Linked List structure
typedef struct {
    node_t* head;
} ll_t;

ll_t* create_list (thread* thr);    // Given thread becomes the head of the least at initialization
ll_t* deep_copy (ll_t* list);       // Deep copies the list itself without deepcopying nodes
// Add an element to the head of the list
void add_to_list (ll_t* list, thread* _thr);
void delete_list (ll_t** list);     // In loop individually free all nodes inside list


// Main structure that represents individual threads
struct Thread {
    int n;              // Number of directly linked subthreads
    ll_t* sub_threads;  // Pointers to those subthreads
    struct Thread* parent; // Ppointer to the parent of this thread
    enum thread_type type; // Determines whether thread is one of the types

    int score;           // Essentially reddit karma
    char content[1024];  // Content of the thread
    char author[32];     // Author of the post
    time_t epoch; // Epoch time for current post
};

thread* create_thread (enum thread_type _type, char _content[1024], char _author[32], thread* _parent);
// Adds an element to sub_threads linked list from head
void add_subthread (thread* thr, thread* sub_thr);

// Individual change to the score of the post. Like == true means upvote, downvote otherwise
void add_score (thread* thr, bool like);

// Given the comparison function, sort the sub-threads of given thread
ll_t* sort_by (thread* thr, int (*cmpfunc) (const void*, const void*));

// Comparison functions for sorting
int by_likes (const void* a, const void* b);     // Sort by score in descending order
int by_date (const void* a, const void* b);      // Sort by date in descending order (new first)
int by_comments (const void* a, const void* b);  // Sort by number of comments in descending order

void remove_thread (thread* thr);  // soft delete of the thread
void delete_thread (thread** thr); // hard delete of the thread

// Saves the entire tree into the file in DFS form
void save_posts (thread* root);
// Saves individual thread into the file and invokes DFS saving for subthreads
void save_thread (FILE *fptr, thread* thr, int depth);

// Loads database from the file and returns the pointer to the root of it
thread* load_database ();
// Helper function to extract current depth from line
int get_depth (FILE *fptr);
// Helper function to extract content
void get_content (FILE *fptr, char (*content)[1024]);
// Helper function to extract author
void get_author (FILE *fptr, char (*author)[32]);

#endif
