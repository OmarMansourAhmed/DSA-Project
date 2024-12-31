#ifndef MUTUALNETWORKS_H  // if not defined
#define MUTUALNETWORKS_H  // if not defined

#include "./xmlInGraph.h"


void printMutualFollowersCaller(const string &file_path , const string &id1, const string &id2, const string &id3);

void printMutualFollowers(const vector<User> &users, const string &id1, const string &id2, const string &id3);

void printSuggestedUsersCaller(const string &file_path , const string &userId);

void printSuggestedUsers(const vector<User> &users, const string &userId);

#endif // MUTUALNETWORKS_H  // if not defined