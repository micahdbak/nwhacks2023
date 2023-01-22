#include "user_base.h"

// Helper function - retrieves 1 entry, either username or password, depending on current position
char* get_word(FILE *fptr) {
    int line_len = 0;
    char num = fgetc(fptr);

    // get length of entry to retrieve, stored as decimal number before entry
    while (num != ' ') {
        line_len = line_len * 10 + (num - '0');
        num = fgetc(fptr);
    }

    // read the entry, using fread to encapsulate all special characters
    char* line = (char*)malloc(line_len+1);
    fread(line, line_len, 1, fptr);

    line[line_len] = '\0';
    return line;
}


int add_user (const char* username, const char* password) {
    FILE *fptr;
    fptr = fopen(FILENAME, "r");
    // skip the search process if file doesn't exist
    if (fptr != NULL) {
        while (fgetc(fptr) != EOF) {
            char* name = get_word(fptr);
            fgetc(fptr);          // skip spacing between entries
            free(get_word(fptr)); // we don't need password, but we still have to offset the pointer

            // if entry already exists
            if (strcmp(name, username) == 0)
                return 0;
            free(name);
        }

        fclose(fptr);
    }
    int user_len = strlen(username);
    int pas_len = strlen(password);

    fptr = fopen(FILENAME, "a");
    fprintf(fptr, "\n%d %s %d %s", user_len, username, pas_len, password);
    fclose(fptr);

    return 1;
}

int find_user (const char* username, const char* password) {
    FILE *fptr;
    fptr = fopen(FILENAME, "r");
    if (fptr == NULL)
        return -1;

    while (fgetc(fptr) != EOF) {
        char* name = get_word(fptr);
        fgetc(fptr); // skip spacing between entries
        char* pas = get_word(fptr);

        // if user exists
        if (strcmp(name, username) == 0) {
            // if passwords match
            if (strcmp(pas, password) == 0) 
                return 1;
            return -3;
        }
        free(name);
        free(pas);
    }

    fclose(fptr);
    return -2;
}

/*int main () {
    const char *username = "Oleg";
    const char *password = "NahuyIdi1";

    add_user (username, password);

    return 0;
}*/