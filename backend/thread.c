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

thread* create_thread (enum thread_type _type, char _content[1024], char _author[32], thread* _parent) {
    thread* thr = (thread*)malloc(sizeof(thread));

    thr->n = 0;
    thr->sub_threads = NULL;
    thr->parent = _parent;
    thr->type = _type;

    thr->score = 0;
    strcpy (thr->content, _content);
    strcpy (thr->author, _author);
    thr->epoch = time(NULL);

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


void remove_thread (thread* thr) {
    strncpy(thr->content, "This post has been deleted by author", 1024);
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

ll_t* sort_by (thread* thr, int (*cmpfunc) (const void*, const void*)) {
    if (thr->sub_threads == NULL)
        return NULL;

    ll_t* copy = deep_copy (thr->sub_threads);
    qsort (copy, thr->n, sizeof(node_t), cmpfunc);

    return copy;
}

int by_likes (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    return thr_b->score - thr_a->score;
}

int by_date (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    return thr_b->epoch - thr_a->epoch;
}

int by_comments (const void* a, const void* b) {
    const thread* thr_a = (thread*) a;
    const thread* thr_b = (thread*) b;

    return thr_b->n - thr_a->n;
}

// ************************************
// Functions for Saving and Loading database (.txt file)
// ************************************

void save_posts (thread* root) {
    FILE *fptr;
    fptr = fopen (FILENAME_DATA, "w");

    if (fptr != NULL) {
        save_thread (fptr, root, 0);        
    }

    fclose (fptr);
}

void save_thread (FILE *fptr, thread* thr, int depth) {
    fprintf (fptr, "\n%d %d~%s~%s~", depth, thr->type, thr->content, thr->author);

    if (thr->sub_threads != NULL) {
        node_t* cur = thr->sub_threads->head;
        while (cur != NULL) {
            save_thread (fptr, cur->thr, depth + 1);
            cur = cur->next;
        }
    }
}


int get_depth (FILE *fptr) {
    int depth = 0;
    int num = fgetc (fptr);
    
    // get depth of entry, stored as decimal number before entry
    while (num != ' ' && num != EOF) {
        depth = depth * 10 + (num - '0');
        num = fgetc (fptr);
    }

    return depth;
}

void get_content (FILE *fptr, char (*content)[1024]) {
    int ch = fgetc (fptr);

    int i;
    for (i = 0; ch != '~' && ch != EOF; i++) {   
        (*content)[i] = ch;
        ch = fgetc (fptr);
    }
    (*content)[i] = '\0';
}

void get_author (FILE *fptr, char (*author)[32]) {
    int ch = fgetc (fptr);

    int i;
    for (i = 0; ch != '~' && ch != EOF; i++) {
        (*author)[i] = ch;
        ch = fgetc (fptr);
    }
    (*author)[i] = '\0';
}

thread* load_database () {
    FILE *fptr;
    fptr = fopen (FILENAME_DATA, "r");
    // If file doesn't exist 
    if (fptr == NULL)
        return NULL;

    int cur_depth = 0, c;
    thread* root = NULL, *cur = root;

    while (fgetc(fptr) != EOF) {
        if ((c = getc(fptr)) == EOF)
            break;
	    //ungetc(c, fptr);

        int depth = get_depth (fptr); // Get the depth from first entry in line
        printf ("%d ", depth);
        // If depth indicates that this is a subpost of latest post -> change directory
        if (depth > cur_depth + 1) {
            cur_depth++;
            cur = cur->sub_threads->head->thr;
        }
        // Otherwise if depth is the same as current depth -> return to parent directory
        else if (depth == cur_depth && depth != 0) {
            cur_depth--;
            cur = cur->parent;
        }
        //printf("Working\n");
        // Get 1-digit type of the thread and cast to enum
        enum thread_type type = (enum thread_type)(fgetc(fptr) - '0');
        fgetc(fptr);   // Skip the ~ divider character
        char content[1024], author[32];
       
        // Fill in content and author from the line
        get_content (fptr, &content);
        get_author (fptr, &author);
        if (strlen(content) == 0 && strlen(author) == 0)
            continue;

        printf ("'%s' '%s'\n", content, author);
        
        // Define the root if it's not yet defined
        if (root == NULL) {
            root = create_thread (type, content, author, NULL);
            cur = root;
        }
        else {
            add_subthread (cur, create_thread (type, content, author, cur));
        }
    }

    fclose (fptr);

    return root;
}


/*int main () {
    thread* root;

    char _content[1024] = "Zalupa sinyaya sialo dike, tvoyu matb ebali koni. Zemlya zelyonaya";
    char _author[32] = "Vlados Pendos";
    root = create_thread (Post, _content, _author, NULL);

    char _content1[1024] = "Na lugu yebali yogikja";
    char _author1[32] = "Andrew";
    thread* thr = create_thread (Post, _content1, _author1, NULL);
    add_subthread (root, thr);

    char _content2[1024] = "Ti pidor";
    char _author2[32] = "Krishna";
    thr = create_thread (Post, _content2, _author2, NULL);
    add_subthread (root, thr);

    char _content3[1024] = "Sam pidor, gandon vonyuchiy";
    char _author3[32] = "Pasha Tehnik";
    thr = create_thread (Post, _content3, _author3, NULL);
    add_subthread (root->sub_threads->head->thr, thr);

    char _content4[1024] = "CMPT";
    char _author4[32] = "Admin";
    thr = create_thread (Folder, _content4, _author4, NULL);
    add_subthread (root, thr);

    char _content5[1024] = "Hello";
    char _author5[32] = "Micah";
    thr = create_thread (Post, _content5, _author5, NULL);
    add_subthread (root->sub_threads->head->thr, thr);

    save_posts (root);

    delete_thread (&root);

    root = load_database ();

    delete_thread (&root);

    return 0;
}*/
