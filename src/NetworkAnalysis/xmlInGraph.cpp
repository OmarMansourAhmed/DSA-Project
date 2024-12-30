#include "../../headers/Parse_XML.h"
#include "../../headers/NetworkAnalysis.h"

vector<User> parseUsersFromXML(const string &file_path)
{
  vector<string> xmlData = Parse_XML_File(file_path);
  vector<User> users;
  User currentUser;
  Posts currentUserPosts;
  bool readingFollowers = false;
  bool readingPosts = false;
  bool readingTopics = false;

  for (unsigned int i = 0; i < xmlData.size(); ++i)
  {
    string tag = xmlData[i];

    if (Get_Type(tag) == 'O') // Opening tag
    {
      string tagName = Get_Tag(tag);
      if (tagName == "user")
      {
        currentUser = User(); // Start a new user
      }
      else if (tagName == "id" && !readingFollowers)
      {
        i++;
        currentUser.id = Erase_Spaces(xmlData[i]);
      }
      else if (tagName == "name")
      {
        i++;
        currentUser.name = xmlData[i];
      }
      else if (tagName == "post")
      {
        currentUserPosts = Posts();
        readingPosts = true;
      }
      else if (tagName == "body" && readingPosts)
      {
        i++;
        currentUserPosts.body = xmlData[i];
      }
      else if (tagName == "follower")
      {
        readingFollowers = true;
      }
      else if (tagName == "topics")
      {
        readingTopics = true;
      }
      else if (tagName == "topic" && readingTopics)
      {
        i++;
        currentUserPosts.topics.push_back(xmlData[i]);
      }
    }
    else if (Get_Type(tag) == 'C') // Closing tag
    {
      string tagName = Get_Tag(tag);
      if (tagName == "/user")
      {
        users.push_back(currentUser);
      }
      else if (tagName == "/post")
      {
        readingPosts = false;
        currentUser.posts.push_back(currentUserPosts);
      }
      else if (tagName == "/topics")
      {
        readingTopics = false;
      }
      else if (tagName == "/follower")
      {
        readingFollowers = false;
      }
    }
    else if (Get_Type(tag) == 'V') // Value tag (data inside tags)
    {
      if (readingFollowers)
      {
        string followerId = Erase_Spaces(tag);
        currentUser.followers.push_back(followerId);
      }
    }
  }

  return users;
}

void generateGraph(const vector<User> &users, const string &outputFile)
{
  ofstream dotFile("graph.dot");
  if (!dotFile.is_open())
  {
    cerr << "Failed to create dot file.\n";
    return;
  }

  dotFile << "digraph SocialNetwork {\n";
  dotFile << "    node [shape=circle, style=filled, color=skyblue, fontname=\"Arial\"];\n";

  for (const auto &user : users)
  {
    dotFile << "    " << user.id << " [label=\"" << user.name << "\"];\n";
    for (const auto &follower : user.followers)
    {
      dotFile << "    " << follower << " -> " << user.id << ";\n";
    }
  }

  dotFile << "}\n";
  dotFile.close();

  string command = "dot -Tjpg graph.dot -o " + outputFile;
  int result = system(command.c_str());
  if (result == 0)
  {
    cout << "Graph image generated successfully: " << outputFile << "\n";
  }
  else
  {
    cerr << "Failed to generate graph image. Ensure Graphviz is installed and accessible.\n";
  }
}
