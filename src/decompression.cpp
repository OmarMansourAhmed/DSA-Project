#include <iostream>
#include <fstream>
#include <sstream>
#include "../headers/decompression.h"
#include <map>

using namespace std;

string decompress(const string &input, const unordered_map<string, char> &mapping)
{
    string data = input;

    // Create a reverse mapping
    map<char, string> reverseMapping;
    for (const auto &[pair, ch] : mapping)
    {
        reverseMapping[ch] = pair;
    }

    // Reverse the replacement
    bool replaced;
    do
    {
        replaced = false;
        for (auto &[ch, pair] : reverseMapping)
        {
            size_t pos;
            while ((pos = data.find(ch)) != string::npos)
            {
                data.replace(pos, 1, pair);
                replaced = true;
            }
        }
    } while (replaced);

    return data;
}
