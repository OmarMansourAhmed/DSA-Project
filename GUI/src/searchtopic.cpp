#include "searchtopic.h"
#include "xmlingraph.h"

void searchTopicCaller(const string &file_path, string topic){
    vector<User> users;
    users = parseUsersFromXML(file_path);
    searchTopic(users, topic);
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
