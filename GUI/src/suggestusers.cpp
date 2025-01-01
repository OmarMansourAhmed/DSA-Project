#include "suggestusers.h"
#include "xmlingraph.h"

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
