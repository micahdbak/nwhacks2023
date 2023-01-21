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
void add_to_list (ll_t* list, thread* _thr);
void delete_list (ll_t** list);


struct Thread {
    int n;              // Number of directly linked subthreads
    ll_t* sub_threads;  // Pointers to those subthreads
    enum thread_type type;
    char content[256];  // Content of the thread
    char author[32];    // Author of the post
    int date[3];        // year/month/day
};
//typedef struct Thread thread;

thread* create_thread (char _content[256], char _author[32], int _date[3]);
void add_subthread (thread* thr, thread* sub_thr);

void remove_thread (thread* thr); // soft delete
void delete_thread (thread** thr); // hard delete


#endif