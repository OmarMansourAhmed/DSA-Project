#include "../../headers/xmlInGraph.h"
#include "../../headers/mutualNetwork.h"



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

void printSuggestedUsersCaller(const string &file_path , const string &userId){
    vector<User> users ;
    users = parseUsersFromXML(file_path) ;
    printSuggestedUsers(users , userId) ;
}

void printSuggestedUsers(const vector<User> &users, const string &userId){
    vector<string> alreadyFollowed; // Track users already followed by the target user

    // Find the target user
    const User *targetUser = nullptr;
    for (const auto &user : users)
    {
        if (user.id == userId)
        {
            targetUser = &user;
            break;
        }
    }

    if (!targetUser)
    {
        cout << "User with ID " << userId << " not found." << endl;
        return;
    }

    // Add the users already followed by the target user to a vector
    for (const auto &followerId : targetUser->followers)
    {
        alreadyFollowed.push_back(followerId);
    }

    // Go through each follower's followers to find suggestions
    for (const auto &followerId : targetUser->followers)
    {
        for (const auto &user : users)
        {
            if (user.id == followerId)
            {
                for (const auto &followedId : user.followers)
                {
                    // Skip if already followed or is the target user
                    if (find(alreadyFollowed.begin(), alreadyFollowed.end(), followedId) == alreadyFollowed.end() && followedId != userId)
                    {
                        // Find and print the name of the suggested user
                        for (const auto &u : users)
                        {
                            if (u.id == followedId)
                            {
                                cout << "ID: " << u.id << ", Name: " << u.name << endl;
                                alreadyFollowed.push_back(followedId); // Avoid duplicates
                                break;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}