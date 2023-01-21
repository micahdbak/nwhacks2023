#include "thread.h"

// ************************************
// Functions for Node struct
// ************************************

node_t* create_node (thread* _thr, node_t* _next) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->thr = _thr;
    new_node->next = _next;
    
    return new_node;
}

void delete_node (node_t* node) {
    free (node->thr);
    free (node);
}


// ************************************
// Functions for LinkedList struct
// ************************************


ll_t* create_list (thread* thr) {
    ll_t* new_list = (ll_t*)malloc(sizeof(new_list));
    new_list->head = create_node (thr);

    return new_list;
}

void add_to_list (ll_t* list, thread* _thr) {
    list->head = create_node(_thr, list->head);
}

void delete_list (ll_t* list) {
    while (list->head != NULL) {
        node_t* temp = list->head;
        list->head = list->head->next;
        delete_node (temp);
    }

    free (list);
}


// ************************************
// Functions for Thread struct
// ************************************

thread* create_thread (char _content[256], char _author[32], int _date[3]) {
    thread* thr = (thread*)malloc(sizeof(thread));
    thr->n = 0;
    thr->sub_threads = NULL;

    int i;
    for (i = 0; _content[i] != '\0'; i++)
        thr->content[i] = _content[i];
    thr->content[i] = '\0';

    for (i = 0; _author[i] != '\0'; i++)
        thr->author[i] = _author[i];
    thr->author[i] = '\0';

    for (i = 0; i < 3; i++)
        thr->date[i] = _date[i];

    return thr;
}

void add_subthread (thread* thr, thread* sub_thr) {
    thr->n++;

    if (thr->sub_threads == NULL)
        thr->sub_threads = create_list (sub_thr);
    else 
        add_to_list (thr->sub_threads, sub_thr);
}
