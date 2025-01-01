#include "level_2.h"
#include "ui_level_2.h"
#include "checkconsistency.h"
#include "mostactive.h"
#include "mostinfluencer.h"
#include "mutualnetwork.h"
#include "searchword.h"
#include "xmlingraph.h"
#include "searchtopic.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>


Level_2::Level_2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Level_2)
{
    ui->setupUi(this);
}

Level_2::~Level_2()
{
    delete ui;
}

void Level_2::on_pushButton_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    try
    {
        // Parse the XML and generate the graph
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        QString outputFile = "output_graph.jpg";
        generateGraph(users, outputFile.toStdString());

        // Check if the image was successfully created
        if (!QFile::exists(outputFile))
        {
            QMessageBox::critical(this, "Error", "Failed to generate graph. Ensure Graphviz is installed.");
            return;
        }

        // Display the graph in a popup
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Generated Graph");
        QVBoxLayout *layout = new QVBoxLayout(dialog);

        QLabel *label = new QLabel(dialog);
        QPixmap pixmap(outputFile);
        if (pixmap.isNull())
        {
            QMessageBox::critical(this, "Error", "Failed to load the graph image.");
            delete dialog;
            return;
        }

        label->setPixmap(pixmap.scaled(800, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout->addWidget(label);

        dialog->setLayout(layout);
        dialog->resize(820, 620);
        dialog->exec();
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}


void Level_2::on_pushButton_2_clicked() //Most Active
{
// Open file dialog to select XML file
QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

if (filePath.isEmpty())
{
    QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
    return;
}

try
{
    // Call the mostActiveUserCaller with the selected file
    vector<User> users = parseUsersFromXML(filePath.toStdString());
    string mostActiveUserName;
        string mostActiveUserId;
    int mostConnections = 0;
    // Compute the most active user
    unordered_map<string, int> connectionsMaps;
    for (const auto &user : users)
    {
        connectionsMaps[user.id] = 0;
    }
    for (const auto &user : users)
   {
           for (const auto &followerId : user.followers)
           {
               if (connectionsMaps.find(followerId) != connectionsMaps.end())
               {
                   connectionsMaps[followerId]++;
               }
           }
           }
           for (const auto &user : users)
           {
               if (connectionsMaps[user.id] > mostConnections)
               {
                   mostConnections = connectionsMaps[user.id];
                   mostActiveUserId = user.id;
                   mostActiveUserName = user.name;
               }
           }
           // Display the most active user in a popup
           QMessageBox::information(this, "Most Active User",
                                    QString("Name: %1\nID: %2\nConnections: %3")
                                        .arg(QString::fromStdString(mostActiveUserName))
                                        .arg(QString::fromStdString(mostActiveUserId))
                                        .arg(mostConnections));
       }
       catch (const std::exception &e)
       {
           QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
       }
   }

void Level_2::on_pushButton_3_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    try
    {
        // Parse users and determine the most influencer user
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        User mostInfluencerUser;
        int maxFollowers = 0;

        for (const auto &user : users)
        {
            int followersCount = user.followers.size();
            if (followersCount > maxFollowers)
            {
                maxFollowers = followersCount;
                mostInfluencerUser = user;
            }
        }

        // Display the influencer user in a popup
        QMessageBox::information(this, "Most Influencer User",
                                 QString("Name: %1\nID: %2\nFollowers: %3")
                                     .arg(QString::fromStdString(mostInfluencerUser.name))
                                     .arg(QString::fromStdString(mostInfluencerUser.id))
                                     .arg(maxFollowers));
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}


void Level_2::on_pushButton_4_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    // Prompt the user for three IDs
    QString id1 = QInputDialog::getText(this, "Enter User ID 1", "User ID 1:");
    if (id1.isEmpty())
    {
        QMessageBox::warning(this, "Missing ID", "User ID 1 is required.");
        return;
    }

    QString id2 = QInputDialog::getText(this, "Enter User ID 2", "User ID 2:");
    if (id2.isEmpty())
    {
        QMessageBox::warning(this, "Missing ID", "User ID 2 is required.");
        return;
    }

    QString id3 = QInputDialog::getText(this, "Enter User ID 3", "User ID 3:");
    if (id3.isEmpty())
    {
        QMessageBox::warning(this, "Missing ID", "User ID 3 is required.");
        return;
    }

    try
    {
        // Call the mutual followers function
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        vector<pair<string, string>> mutualFollowers;

        vector<string> userIds = {id1.toStdString(), id2.toStdString(), id3.toStdString()};
        for (const string &id : userIds)
        {
            auto userIt = find_if(users.begin(), users.end(), [&id](const User &user) { return user.id == id; });

            if (userIt != users.end())
            {
                const vector<string> &followers = userIt->followers;

                if (mutualFollowers.empty())
                {
                    for (const string &followerId : followers)
                    {
                        auto followerIt = find_if(users.begin(), users.end(), [&followerId](const User &user) { return user.id == followerId; });
                        if (followerIt != users.end())
                        {
                            mutualFollowers.emplace_back(followerIt->id, followerIt->name);
                        }
                    }
                }
                else
                {
                    vector<pair<string, string>> temp;
                    for (const auto &follower : mutualFollowers)
                    {
                        auto followerIt = find_if(users.begin(), users.end(), [&follower](const User &user) { return user.id == follower.first; });
                        if (followerIt != users.end() && ::find(followers.begin(), followers.end(), followerIt->id) != followers.end())
                        {
                            temp.push_back(follower);
                        }
                    }
                    mutualFollowers = temp;
                }
            }
        }

        // Prepare the result string
        QString result = "Mutual Followers:\n";
        for (const auto &follower : mutualFollowers)
        {
            result += QString("ID: %1, Name: %2\n").arg(QString::fromStdString(follower.first)).arg(QString::fromStdString(follower.second));
        }

        if (mutualFollowers.empty())
        {
            result = "No mutual followers found.";
        }

        // Display the result in a popup
        QMessageBox::information(this, "Mutual Followers", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}



void Level_2::on_pushButton_5_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    // Prompt the user for a single ID
    QString userId = QInputDialog::getText(this, "Enter User ID", "User ID:");
    if (userId.isEmpty())
    {
        QMessageBox::warning(this, "Missing ID", "User ID is required.");
        return;
    }

    try
    {
        // Parse users and calculate suggested users
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        vector<string> alreadyFollowed;
        vector<pair<string, string>> suggestedUsers;

        // Find the target user
        const User *targetUser = nullptr;
        for (const auto &user : users)
        {
            if (user.id == userId.toStdString())
            {
                targetUser = &user;
                break;
            }
        }

        if (!targetUser)
        {
            QMessageBox::warning(this, "User Not Found", "The user with the specified ID was not found.");
            return;
        }

        // Track users already followed
        alreadyFollowed.insert(alreadyFollowed.end(), targetUser->followers.begin(), targetUser->followers.end());

        // Find suggested users
        for (const auto &followerId : targetUser->followers)
        {
            for (const auto &user : users)
            {
                if (user.id == followerId)
                {
                    for (const auto &followedId : user.followers)
                    {
                        if (::find(alreadyFollowed.begin(), alreadyFollowed.end(), followedId) == alreadyFollowed.end() && followedId != targetUser->id)
                        {
                            auto it = find_if(users.begin(), users.end(), [&followedId](const User &u) { return u.id == followedId; });
                            if (it != users.end())
                            {
                                suggestedUsers.emplace_back(it->id, it->name);
                                alreadyFollowed.push_back(followedId);
                            }
                        }
                    }
                    break;
                }
            }
        }

        // Prepare the result string
        QString result;
        if (!suggestedUsers.empty())
        {
            result = "Suggested Users:\n";
            for (const auto &suggested : suggestedUsers)
            {
                result += QString("ID: %1, Name: %2\n").arg(QString::fromStdString(suggested.first)).arg(QString::fromStdString(suggested.second));
            }
        }
        else
        {
            result = "No suggested users found.";
        }

        // Display the result in a popup
        QMessageBox::information(this, "Suggested Users", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}


void Level_2::on_pushButton_6_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    // Prompt the user for the search word
    QString word = QInputDialog::getText(this, "Enter Word", "Word to Search:");
    if (word.isEmpty())
    {
        QMessageBox::warning(this, "Missing Word", "A word is required for the search.");
        return;
    }

    try
    {
        // Parse users and perform the search
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        vector<pair<string, string>> matchingUsers;

        for (const auto &user : users)
        {
            for (const auto &post : user.posts)
            {
                if (post.body.find(word.toStdString()) != string::npos)
                {
                    matchingUsers.emplace_back(user.id, user.name);
                    break; // Avoid duplicate entries for the same user
                }
            }
        }

        // Prepare the result string
        QString result;
        if (!matchingUsers.empty())
        {
            result = "Users with posts containing the word:\n";
            for (const auto &user : matchingUsers)
            {
                result += QString("ID: %1, Name: %2\n").arg(QString::fromStdString(user.first)).arg(QString::fromStdString(user.second));
            }
        }
        else
        {
            result = "No users found with posts containing the word.";
        }

        // Display the result in a popup
        QMessageBox::information(this, "Search Results", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}


void Level_2::on_pushButton_7_clicked()
{
    // Open file dialog to select XML file
    QString filePath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml);;All Files (*)");

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "No File Selected", "Please select an XML file.");
        return;
    }

    // Prompt the user for the topic
    QString topic = QInputDialog::getText(this, "Enter Topic", "Topic to Search:");
    if (topic.isEmpty())
    {
        QMessageBox::warning(this, "Missing Topic", "A topic is required for the search.");
        return;
    }

    try
    {
        // Parse users and perform the search
        vector<User> users = parseUsersFromXML(filePath.toStdString());
        vector<pair<string, string>> matchingUsers;

        for (const auto &user : users)
        {
            for (const auto &post : user.posts)
            {
                if (::find(post.topics.begin(), post.topics.end(), topic.toStdString()) != post.topics.end())
                {
                    matchingUsers.emplace_back(user.id, user.name);
                    break; // Avoid duplicate entries for the same user
                }
            }
        }

        // Prepare the result string
        QString result;
        if (!matchingUsers.empty())
        {
            result = "Users with posts containing the topic:\n";
            for (const auto &user : matchingUsers)
            {
                result += QString("ID: %1, Name: %2\n").arg(QString::fromStdString(user.first)).arg(QString::fromStdString(user.second));
            }
        }
        else
        {
            result = "No users found with posts containing the topic.";
        }

        // Display the result in a popup
        QMessageBox::information(this, "Search Results", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}

