#include "../../headers/Parse_XML.h"
#include "../../headers/xmlInGraph.h"
#include "../../headers/NetworkAnalysis.h"

void mostInfluencerCaller(const string &file_path)
{
    vector<User> users;
    users = parseUsersFromXML(file_path);
    mostInfluencer(users);
}

void mostInfluencer(const vector<User> &users)
{
    User mostInfluencerUser = User();
    int currentfollowers = 0;
    int most = 0;
    for (int i = 0; i < users.size(); i++)
    {
        currentfollowers = users[i].followers.size();
        if (currentfollowers > most)
        {
            most = currentfollowers;
            mostInfluencerUser = users[i];
        }
    }
    cout << "Name: " << mostInfluencerUser.name << endl;
    cout << "id: " << mostInfluencerUser.id << endl;
}

void mostActiveUserCaller(const string &file_path)
{
    vector<User> users;
    users = parseUsersFromXML(file_path);
    mostActiveUser(users);
}

void mostActiveUser(const vector<User> &users)
{
    unordered_map<string, int> connectionsMaps;
    string mostActiveUserId;
    string mostActiveUserName;
    int mostConnections = 0;

    for (int i = 0; i < users.size(); i++)
    {
        connectionsMaps[users[i].id] = 0;
    }

    for (int i = 0; i < users.size(); i++)
    {
        for (const auto &followerId : users[i].followers)
        {
            if (connectionsMaps.find(followerId) != connectionsMaps.end())
            {
                connectionsMaps[followerId]++;
            }
        }
    }

    for (int i = 0; i < users.size(); i++)
    {
        if (connectionsMaps[users[i].id] > mostConnections)
        {
            mostConnections = connectionsMaps[users[i].id];
            mostActiveUserId = users[i].id;
            mostActiveUserName = users[i].name;
        }
    }

    cout << "Name: " << mostActiveUserName << endl;
    cout << "id: " << mostActiveUserId << endl;
}