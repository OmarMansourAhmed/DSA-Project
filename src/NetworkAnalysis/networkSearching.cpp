#include "../../headers/xmlInGraph.h"
#include "../../headers/networkSearching.h"




void searchWordCaller(const string &file_path, string word){
    vector<User> users;
    users = parseUsersFromXML(file_path);
    searchWord(users,word);
}


void searchTopicCaller(const string &file_path, string topic){
    vector<User> users;
    users = parseUsersFromXML(file_path);
    searchTopic(users, topic);
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

// Function to search posts for a specific topic
void searchTopic(const vector<User> &users, const string &topic)
{
    cout << "Posts containing the topic "" << topic << "":" << endl;

    for (const auto &user : users)
    {
        for (const auto &post : user.posts)
        {
            if (find(post.topics.begin(), post.topics.end(), topic) != post.topics.end())
            {
                cout << "User: " << user.name << " (ID: " << user.id << ")\n";
                cout << "Post Body: " << post.body << "\n\n";
            }
        }
    }
}