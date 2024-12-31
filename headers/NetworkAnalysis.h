#ifndef NETWORK_ANALYSIS_H
#define NETWORK_ANALYSIS_H

#include <vector>
#include <string>
#include <map>
class User;
// Forward declarations of the network analysis functions
void mostInfluencer(const std::vector<User> &users);
void mostActiveUser(const std::vector<User> &users);

#endif // NETWORK_ANALYSIS_H