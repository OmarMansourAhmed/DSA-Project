#ifndef SUGGESTUSERS_H
#define SUGGESTUSERS_H

#include "xmlingraph.h"

void printSuggestedUsersCaller(const string &file_path , const string &userId);

void printSuggestedUsers(const vector<User> &users, const string &userId);

#endif // SUGGESTUSERS_H
