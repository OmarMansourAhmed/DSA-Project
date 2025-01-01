#ifndef MUTUALNETWORK_H
#define MUTUALNETWORK_H

#include "xmlingraph.h"

void printMutualFollowersCaller(const string &file_path , const string &id1, const string &id2, const string &id3);

void printMutualFollowers(const vector<User> &users, const string &id1, const string &id2, const string &id3);

#endif // MUTUALNETWORK_H
