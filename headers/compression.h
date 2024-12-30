#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <string>
#include <unordered_map>

std::unordered_map<std::string, int> countPairs(const std::string &input);
std::string replacePair(const std::string &input, const std::string &pair, char newChar);
std::string compress(const std::string &input, int iterations, std::unordered_map<std::string, char> &mapping);

#endif // COMPRESSION_H
