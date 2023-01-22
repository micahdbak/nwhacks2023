#ifndef USERS_H
#define USERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "user_base.h"

#define FILENAME "users.txt"

// Adds user and his password to the .txt database file
// stored in form:  n1 login n2 password\n
// where [n1 is length of login] and [n2 is length of password]
// returns 0 for already created username, 1 for successfuly created user
int add_user (const char* username, const char* password);

// Finds user in the .txt database file and compares given password with the stored
// Returns 1 for successful login, -1 if file doesn't exist, -2 for wrong username, -3 for wrong passowrd
int find_user (const char* username, const char* password);

#endif