#include "../headers/compression.h"
#include <algorithm>

using namespace std;

unordered_map<string, int> countPairs(const string &input)
{
    unordered_map<string, int> pairCounts;
    for (size_t i = 0; i < input.size() - 1; ++i)
    {
        string pair = input.substr(i, 2);
        pairCounts[pair]++;
    }
    return pairCounts;
}

string replacePair(const string &input, const string &pair, char newChar)
{
    string result;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (i < input.size() - 1 && input.substr(i, 2) == pair)
        {
            result += newChar;
            ++i;
        }
        else
        {
            result += input[i];
        }
    }
    return result;
}

string compress(const string &input, int iterations, unordered_map<string, char> &mapping)
{
    string data = input;
    unsigned char nextChar = 128;

    for (int i = 0; i < iterations; ++i)
    {
        auto pairCounts = countPairs(data);
        if (pairCounts.empty())
            break;

        auto maxPair = max_element(pairCounts.begin(), pairCounts.end(),
                                   [](const auto &a, const auto &b)
                                   {
                                       return a.second < b.second;
                                   });

        string mostFrequentPair = maxPair->first;
        if (mapping.find(mostFrequentPair) != mapping.end())
            break;

        mapping[mostFrequentPair] = nextChar;
        data = replacePair(data, mostFrequentPair, nextChar);
        nextChar++;
    }

    return data;
}
