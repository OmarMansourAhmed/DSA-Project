#ifndef NETWORKANALYSIS_H
#define NETWORKANALYSIS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
using namespace std;

struct Posts
{
public:
  string body;
  vector<string> topics;
};
struct User
{
public:
  string id;
  string name;
  vector<string> followers;
  vector<Posts> posts;
};

vector<User> parseUsersFromXML(const string &file_path);

    void generateGraph(const vector<User> &users, const string &outputFile);

#endif // NETWORKANALYSIS_H