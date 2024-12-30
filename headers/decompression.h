#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <string>
#include <unordered_map>

std::string decompress(const std::string &input, const std::unordered_map<std::string, char> &mapping);

#endif // DECOMPRESSION_H
