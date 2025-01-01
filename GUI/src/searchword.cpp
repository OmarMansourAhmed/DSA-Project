#include "searchword.h"

#include "xmlingraph.h"

void searchWordCaller(const string &file_path, string word){
    vector<User> users;
    users = parseUsersFromXML(file_path);
    searchWord(users,word);
}

// Function to search posts for a specific word
void searchWord(const vector<User> &users, const string &word)
{
    cout << "Posts containing the word "" << word << "":" << endl;

    for (const auto &user : users)
    {
        for (const auto &post : user.posts)
        {
            if (post.body.find(word) != string::npos)
            {
                cout << "User: " << user.name << " (ID: " << user.id << ")\n";
                cout << "Post Body: " << post.body << "\n\n";
            }
        }
    }
}
