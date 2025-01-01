#include "level_2_cp.h"
#include "ui_level_2_cp.h"
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


level_2_CP::level_2_CP(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::level_2_CP)
{
    ui->setupUi(this);
}

level_2_CP::~level_2_CP()
{
    delete ui;
}


void level_2_CP::on_pushButton_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Parse the XML and generate the graph
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());
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

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_2_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Parse the XML and find the most active user
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());

        // Logic to find the most active user
        unordered_map<string, int> connectionsMap;
        string mostActiveUserId;
        string mostActiveUserName;
        int mostConnections = 0;

        for (const auto &user : users)
        {
            connectionsMap[user.id] = 0;
        }

        for (const auto &user : users)
        {
            for (const auto &followerId : user.followers)
            {
                if (connectionsMap.find(followerId) != connectionsMap.end())
                {
                    connectionsMap[followerId]++;
                }
            }
        }

        for (const auto &user : users)
        {
            if (connectionsMap[user.id] > mostConnections)
            {
                mostConnections = connectionsMap[user.id];
                mostActiveUserId = user.id;
                mostActiveUserName = user.name;
            }
        }

        // Display the most active user
        if (!mostActiveUserName.empty())
        {
            QMessageBox::information(this, "Most Active User",
                                     QString("Name: %1\nID: %2\nConnections: %3")
                                         .arg(QString::fromStdString(mostActiveUserName))
                                         .arg(QString::fromStdString(mostActiveUserId))
                                         .arg(mostConnections));
        }
        else
        {
            QMessageBox::information(this, "Most Active User", "No active user found.");
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_15_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Parse the XML and find the most influential user
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());

        // Logic to find the most influential user
        User mostInfluentialUser;
        int maxFollowers = 0;

        for (const auto &user : users)
        {
            int followersCount = user.followers.size();
            if (followersCount > maxFollowers)
            {
                maxFollowers = followersCount;
                mostInfluentialUser = user;
            }
        }

        // Display the most influential user
        if (!mostInfluentialUser.name.empty())
        {
            QMessageBox::information(this, "Most Influential User",
                                     QString("Name: %1\nID: %2\nFollowers: %3")
                                         .arg(QString::fromStdString(mostInfluentialUser.name))
                                         .arg(QString::fromStdString(mostInfluentialUser.id))
                                         .arg(maxFollowers));
        }
        else
        {
            QMessageBox::information(this, "Most Influential User", "No influential user found.");
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_16_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
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

        // Parse the XML and find mutual followers
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());
        vector<pair<string, string>> mutualFollowers;

        vector<string> userIds = {id1.toStdString(), id2.toStdString(), id3.toStdString()};

        for (const auto &id : userIds)
        {
            auto userIt = find_if(users.begin(), users.end(), [&id](const User &user) { return user.id == id; });

            if (userIt != users.end())
            {
                const vector<string> &followers = userIt->followers;

                if (mutualFollowers.empty())
                {
                    for (const auto &followerId : followers)
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
        QString result;
        if (!mutualFollowers.empty())
        {
            result = "Mutual Followers:\n";
            for (const auto &follower : mutualFollowers)
            {
                result += QString("ID: %1, Name: %2\n").arg(QString::fromStdString(follower.first)).arg(QString::fromStdString(follower.second));
            }
        }
        else
        {
            result = "No mutual followers found.";
        }

        // Display the result
        QMessageBox::information(this, "Mutual Followers", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_13_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Prompt the user for a single user ID
        QString userId = QInputDialog::getText(this, "Enter User ID", "User ID:");
        if (userId.isEmpty())
        {
            QMessageBox::warning(this, "Missing ID", "User ID is required.");
            return;
        }

        // Parse the XML and find suggested users
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());
        vector<string> alreadyFollowed;
        vector<pair<string, string>> suggestedUsers;

        // Find the target user
        auto targetUserIt = find_if(users.begin(), users.end(), [&userId](const User &user) { return user.id == userId.toStdString(); });

        if (targetUserIt == users.end())
        {
            QMessageBox::information(this, "User Not Found", "The user with the specified ID was not found.");
            return;
        }

        const User &targetUser = *targetUserIt;

        // Track users already followed by the target user
        alreadyFollowed.insert(alreadyFollowed.end(), targetUser.followers.begin(), targetUser.followers.end());

        // Find suggested users
        for (const auto &followerId : targetUser.followers)
        {
            auto followerIt = find_if(users.begin(), users.end(), [&followerId](const User &user) { return user.id == followerId; });

            if (followerIt != users.end())
            {
                for (const auto &followedId : followerIt->followers)
                {
                    if (::find(alreadyFollowed.begin(), alreadyFollowed.end(), followedId) == alreadyFollowed.end() && followedId != targetUser.id)
                    {
                        auto suggestedIt = find_if(users.begin(), users.end(), [&followedId](const User &user) { return user.id == followedId; });

                        if (suggestedIt != users.end())
                        {
                            suggestedUsers.emplace_back(suggestedIt->id, suggestedIt->name);
                            alreadyFollowed.push_back(followedId); // Avoid duplicates
                        }
                    }
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

        // Display the result
        QMessageBox::information(this, "Suggested Users", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_14_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Prompt the user for the search word
        QString word = QInputDialog::getText(this, "Enter Word", "Word to Search:");
        if (word.isEmpty())
        {
            QMessageBox::warning(this, "Missing Word", "A word is required for the search.");
            return;
        }

        // Parse the XML and search for the word
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());
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

        // Display the result
        QMessageBox::information(this, "Search Results", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}


void level_2_CP::on_pushButton_17_clicked()
{
    // Get text from QPlainTextEdit
    QString xmlContent = ui->plainTextEdit->toPlainText();

    if (xmlContent.isEmpty())
    {
        QMessageBox::warning(this, "Empty Input", "Please enter XML content in the text box.");
        return;
    }

    // Save the XML content to a temporary file
    QString tempFilePath = "temp_input.xml";
    QFile tempFile(tempFilePath);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to create a temporary file for the XML content.");
        return;
    }

    QTextStream out(&tempFile);
    out << xmlContent;
    tempFile.close();

    try
    {
        // Prompt the user for the search topic
        QString topic = QInputDialog::getText(this, "Enter Topic", "Topic to Search:");
        if (topic.isEmpty())
        {
            QMessageBox::warning(this, "Missing Topic", "A topic is required for the search.");
            return;
        }

        // Parse the XML and search for the topic
        vector<User> users = parseUsersFromXML(tempFilePath.toStdString());
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

        // Display the result
        QMessageBox::information(this, "Search Results", result);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }

    // Clean up the temporary file
    tempFile.remove();
}

