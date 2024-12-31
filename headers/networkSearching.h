#ifndef NETWORKSEARCHING_H
#define NETWORKSEARCHING_H
#include "./xmlInGraph.h"


void searchWord(const vector<User> &users, const string &word);
void searchTopic(const vector<User> &users, const string &topic);


void searchWordCaller(const string &file_path, string word);
void searchTopicCaller(const string &file_path, string topic);




#endif 