#include "mostinfluencer.h"
#include "xmlingraph.h"

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
