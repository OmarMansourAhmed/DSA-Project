#include "mutualnetwork.h"
#include "xmlingraph.h"

void printMutualFollowersCaller(const string &file_path , const string &id1, const string &id2, const string &id3){
    vector<User> users ;
    users = parseUsersFromXML(file_path) ;
    printMutualFollowers(users , id1 , id2 , id3) ;
}

void printMutualFollowers(const vector<User> &users, const string &id1, const string &id2, const string &id3){

    vector<pair<string, string>> mutualFollowers;

    vector<string> userIds = {id1, id2, id3}; // Store the IDs in a vector for iteration

    for (const string &id : userIds)
    {
        // Find the user with the given ID
        auto userIt = find_if(users.begin(), users.end(), [&id](const User &user) { return user.id == id; });

        if (userIt != users.end())
        {
            const vector<string> &followers = userIt->followers;

            if (mutualFollowers.empty())
            {
                // Initialize mutualFollowers with the first user's followers
                for (const string &followerId : followers)
                {
                    // Find follower details
                    auto followerIt = find_if(users.begin(), users.end(), [&followerId](const User &user) { return user.id == followerId; });
                    if (followerIt != users.end())
                    {
                        mutualFollowers.emplace_back(followerIt->id, followerIt->name);
                    }
                }
            }
            else
            {
                // Retain only common followers
                vector<pair<string, string>> temp;
                for (const auto &follower : mutualFollowers)
                {
                    auto followerIt = find_if(users.begin(), users.end(), [&follower](const User &user) { return user.id == follower.first; });
                    if (followerIt != users.end() && find(followers.begin(), followers.end(), followerIt->id) != followers.end())
                    {
                        temp.push_back(follower);
                    }
                }
                mutualFollowers = temp;
            }
        }
    }

    // Print the mutual followers
    for (const auto &follower : mutualFollowers)
    {
        cout << "ID: " << follower.first << ", Name: " << follower.second << endl;
    }
}
