/*************************
 * backend: The backend. *
 * authors: Micah & Vlad *
 *************************/

#include "thread.h"

// various functions for the manipulation of threads

thread *node_at_path(thread *node, const char *path);
void list_ll(ll_t *list, char *reply);

void cmd_list(thread *root, const char *path, char *reply);
void cmd_view(thread *node, const char *path, char *reply);
