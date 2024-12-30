#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include "../headers/prettifying_XML.h"
#include <sstream>
using namespace std;

string trimWhitespace(const string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == string::npos || last == string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

void prettifyXML(const string &inputFile, const string &outputFile)
{
    ifstream input(inputFile);
    ofstream output(outputFile);

    if (!input.is_open() || !output.is_open())
    {
        cout << "Can't open the input or the output file.\nTry Again!!" << endl;
        return;
    }

    stack<string> tagStack;
    string currentContent;
    string formatted_XML;

    // Read entire file into a string
    stringstream buffer;
    buffer << input.rdbuf();
    string content = buffer.str();

    for (size_t i = 0; i < content.length(); i++)
    {
        char c = content[i];

        if (c == '<')
        {
            // Handle any text content before the tag
            string trimmed = trimWhitespace(currentContent);
            if (!trimmed.empty())
            {
                formatted_XML += string(tagStack.size(), '\t') + trimmed + "\n";
            }
            currentContent = c;
        }
        else if (c == '>')
        {
            currentContent += c;
            string tagContent = trimWhitespace(currentContent);

            if (tagContent[1] == '/') // Closing tag
            {
                if (!tagStack.empty())
                {
                    formatted_XML += string(tagStack.size() - 1, '\t') + tagContent + "\n";
                    tagStack.pop();
                }
            }
            else if (tagContent[tagContent.length() - 2] == '/') // Self-closing tag
            {
                formatted_XML += string(tagStack.size(), '\t') + tagContent + "\n";
            }
            else // Opening tag
            {
                formatted_XML += string(tagStack.size(), '\t') + tagContent + "\n";
                tagStack.push(tagContent);
            }
            currentContent.clear();
        }
        else
        {
            currentContent += c;
        }
    }

    // Handle any remaining content
    string trimmed = trimWhitespace(currentContent);
    if (!trimmed.empty())
    {
        formatted_XML += string(tagStack.size(), '\t') + trimmed + "\n";
    }

    output << formatted_XML;
    input.close();
    output.close();
    cout << "XML prettified successfully and saved." << endl;
}