#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "thread.h"


enum thread_type {Comment, Post, Folder};

struct Thread;
typedef struct Thread thread;

struct Node {
    thread* thr;
    struct Node* next;
};
typedef struct Node node_t;

node_t* create_node (thread* _thr, node_t* _next);
void delete_node (node_t** node);


typedef struct {
    node_t* head;
} ll_t;

ll_t* create_list (thread* thr);
ll_t* deep_copy (ll_t* list);       // Deep copies the list itself without deepcopying nodes
void add_to_list (ll_t* list, thread* _thr);
void delete_list (ll_t** list);


struct Thread {
    int n;              // Number of directly linked subthreads
    ll_t* sub_threads;  // Pointers to those subthreads
    enum thread_type type;

    char content[1024];  // Content of the thread
    char author[32];    // Author of the post
    int date[4];        // year/month/day/seconds since beginning of day
    int score;
};

thread* create_thread (char _content[1024], char _author[32], int _date[4]);
void add_subthread (thread* thr, thread* sub_thr);

void add_score (thread* thr, bool like);

// Comparison functions for sorting
int by_likes (const void* a, const void* b);    // Sort by score in descending order
int by_date (const void* a, const void* b);     // Sort by date in descending order (new first)
int by_comments (const void* a, const void* b); // Sort by number of comments in descending order

// Given the comparison function, sort the sub-threads of given thread
ll_t* sort_by (thread* thr, int (*cmpfunc) (const void*, const void*));

void remove_thread (thread* thr); // soft delete
void delete_thread (thread** thr); // hard delete


#endif
