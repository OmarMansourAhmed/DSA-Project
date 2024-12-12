#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <fstream>


using namespace std;

void convertToJSON(const string &inputFile, const string &outputFile)
{
  ifstream input(inputFile);
  ofstream output(outputFile);

  if (!input.is_open() || !output.is_open())
  {
    cout << "Can't open the input or the output file.\nTry Again!!" << endl;
    return;
  }

  stack<pair<string, bool>> tags; // Pair of tag name and isFirstChild flag
  string line;
  string json = "{\n";
  string currentContent;

  while (getline(input, line))
  {
    // Remove leading and trailing spaces
    line = regex_replace(line, regex("^\\s+|\\s+$"), "");

    if (line.empty())
      continue;

    for (size_t i = 0; i < line.size(); i++)
    {
      if (line[i] == '<')
      {
        // Output current content if available
        if (!currentContent.empty())
        {
          json += string(tags.size() + 1, '\t') + "\"value\": \"" + currentContent + "\"\n";
          currentContent.clear();
        }

        size_t closePos = line.find('>', i);
        if (closePos == string::npos)
          break;

        string tag = line.substr(i + 1, closePos - i - 1);
        i = closePos;

        if (tag[0] == '/')
        {
          // Closing tag
          json += string(tags.size(), '\t') + "}\n";
          tags.pop();

          if (!tags.empty())
          {
            auto &[parentTag, isFirstChild] = tags.top();
            isFirstChild = false;
          }
        }
        else
        {
          // Opening or self-closing tag
          bool selfClosing = (tag.back() == '/');
          if (selfClosing)
            tag.pop_back();

          if (!tags.empty() && !tags.top().second)
          {
            json += string(tags.size(), '\t') + ",\n";
            auto &[parentTag, isFirstChild] = tags.top();
            isFirstChild = true;
          }

          json += string(tags.size(), '\t') + "\"" + tag + "\": {\n";
          tags.push({tag, true});

          if (selfClosing)
          {
            json += string(tags.size(), '\t') + "}\n";
            tags.pop();

            if (!tags.empty())
            {
              auto &[parentTag, isFirstChild] = tags.top();
              isFirstChild = false;
            }
          }
        }
      }
      else
      {
        currentContent += line[i];
      }
    }
  }

  // Close any remaining open tags
  while (!tags.empty())
  {
    json += string(tags.size(), '\t') + "}\n";
    tags.pop();
  }

  json += "}\n";
  output << json;

  input.close();
  output.close();

  cout << "XML converted to JSON successfully and saved in: \n"
       << outputFile << endl;
}

// int main()
// {
//   string inputFile = "D:/kolya/Fall 24/Data Structure/Project/Code/DSA-Project/sample.xml";
//   string outputFile = "D:/kolya/Fall 24/Data Structure/Project/Code/DSA-Project/output.json";

//   convertToJSON(inputFile, outputFile);

//   return 0;
// }