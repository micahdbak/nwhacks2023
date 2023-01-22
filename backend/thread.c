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

// Forward declaration
void delete_thread (thread** thr);

void delete_node (node_t** node) {
    if (*node == NULL)
        return;

    delete_thread (&( (*node)->thr ));
    free (*node);
    *node = NULL;
}


// ************************************
// Functions for LinkedList struct
// ************************************


ll_t* create_list (thread* thr) {
    ll_t* new_list = (ll_t*)malloc(sizeof(ll_t));
    new_list->head = create_node (thr, NULL);

    return new_list;
}

ll_t* deep_copy (ll_t* list) {
    ll_t* copy = (ll_t*)malloc(sizeof(ll_t));
    
    node_t* cur = list->head;
    while (cur != NULL) {
        add_to_list (copy, cur->thr);
        cur = cur->next;
    }

    return copy;
}

void add_to_list (ll_t* list, thread* _thr) {
    list->head = create_node(_thr, list->head);
}

void delete_list (ll_t** list) {
    if (*list == NULL)
        return;

    while ((*list)->head != NULL) {
        node_t* temp = (*list)->head;
        (*list)->head = (*list)->head->next;
        delete_node (&temp);
    }

    free (*list);
    *list = NULL;
}


// ************************************
// Functions for Thread struct
// ************************************

thread* create_thread (char _content[1024], char _author[32], int _date[3]) {
    thread* thr = (thread*)malloc(sizeof(thread));
    thr->n = 0;
    thr->sub_threads = NULL;
    thr->score = 0;

    int i;
    for (i = 0; _content[i] != '\0'; i++)
        thr->content[i] = _content[i];
    thr->content[i] = '\0';

    for (i = 0; _author[i] != '\0'; i++)
        thr->author[i] = _author[i];
    thr->author[i] = '\0';

    for (i = 0; i < 4; i++)
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


void add_score (thread* thr, bool like) {
    thr->score += (like) ? 1 : -1;
}

ll_t* sort_by (thread* thr, int (*cmpfunc) (const void*, const void*)) {
    if (thr->sub_threads == NULL)
        return;

    ll_t* copy = deep_copy (thr->sub_threads);
    qsort (copy, thr->n, sizeof(node_t), cmpfunc);

    return copy;
}


void remove_thread (thread* thr) {
    strncpy(thr->content, "This post has been deleted by author", 256);
}

void delete_thread (thread** thr) {
    if (*thr == NULL)
        return;

    delete_list (&( (*thr)->sub_threads ));
    free (*thr);
    *thr = NULL;
}


// ************************************
// Functions for Sorting threads
// ************************************

int by_likes (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    return thr_b->score - thr_a->score;
}

int by_date (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    // Skip to month if years are the same
    if (thr_b->date[0] != thr_a->date[0])
        return thr_b->date[0] - thr_a->date[0];
    // Skip to day if months are the same
    if (thr_b->date[1] != thr_a->date[1])
        return thr_b->date[1] - thr_a->date[1];
    // Skip to seconds if days are the same
    if (thr_b->date[2] != thr_a->date[2])
        return thr_b->date[2] - thr_a->date[2];
    // Compare by seconds if all other values are the same
    return thr_b->date[3] - thr_a->date[3];
}

int by_comments (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    return thr_b->n - thr_a->n;
}


int main () {
    int _date[4] = {10, 1, 2, 1000};
    char _content[1024] = "AUHEUFBEIE";
    char _author[32] = "WNIUFJFJWNEO";

    thread* thr = create_thread (_content, _author, _date);

    node_t* test = create_node (thr, NULL);
    delete_thread (&thr);
    delete_node (&test);

    return 0;
}
