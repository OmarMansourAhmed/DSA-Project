#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>
#include "../headers/ConvertToJSON.h"

using namespace std;

// Helper function to trim whitespace from a string
string trim(const string &str) {
    int start = str.find_first_not_of(" \t\n\r");
    int end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Function to parse an XML input stream into an XmlNode tree
XmlNode parseXml(istream &input)
{
  string line, tag;
  XmlNode root;                // Root node of the XML
  vector<XmlNode *> nodeStack; // Stack to keep track of nested nodes

  while (getline(input, line))
  {
    line = trim(line); // Remove leading/trailing whitespace
    if (line.empty())
      continue;

    if (line[0] == '<' && line[1] != '/')
    { // Start tag
      int tagEnd = line.find('>');
      tag = line.substr(1, tagEnd - 1);

      XmlNode node;
      int spacePos = tag.find(' ');

      // Parse tag name and attributes
      if (spacePos != string::npos)
      {
        node.name = tag.substr(0, spacePos);
        string attributesStr = tag.substr(spacePos + 1);
        int eq, quote1, quote2;

        while ((eq = attributesStr.find('=')) != string::npos)
        {
          quote1 = attributesStr.find('"', eq);
          quote2 = attributesStr.find('"', quote1 + 1);
          string attrName = trim(attributesStr.substr(0, eq));
          string attrValue = attributesStr.substr(quote1 + 1, quote2 - quote1 - 1);
          node.attributes[attrName] = attrValue;
          attributesStr = attributesStr.substr(quote2 + 1);
        }
      }
      else
      {
        node.name = tag; // No attributes
      }

      // Attach the node to its parent or set it as root
      if (!nodeStack.empty())
      {
        nodeStack.back()->children.push_back(node);
        nodeStack.push_back(&nodeStack.back()->children.back());
      }
      else
      {
        root = node;
        nodeStack.push_back(&root);
      }
    }
    else if (line[0] == '<' && line[1] == '/')
    { // End tag
      nodeStack.pop_back();
    }
    else
    { // Text node
      if (!nodeStack.empty())
      {
        nodeStack.back()->text += line;
      }
    }
  }

  return root;
}

// Function to convert an XmlNode tree into JSON format
string xmlToJson(const XmlNode &node, int indent)
{
  ostringstream oss;
  string indentation(indent, ' ');

  oss << "{\n";
  oss << indentation << "  \"name\": \"" << node.name << "\",\n";

  // Add attributes if present
  if (!node.attributes.empty())
  {
    oss << indentation << "  \"attributes\": {\n";
    bool firstAttr = true;
    for (const auto &attr : node.attributes)
    {
      if (!firstAttr) oss << ",\n";
      oss << indentation << "  \"@" << attr.first << "\": \"" << attr.second << "\"";
      firstAttr = false;
    }
    oss << "\n" << indentation << "  },\n";
  }

  // Add text if present
  if (!node.text.empty())
  {
    oss << indentation << "  \"text\": \"" << trim(node.text) << "\",\n";
  }

  // Add children if present
  if (!node.children.empty())
  {
    oss << indentation << "  \"children\": [\n";
    for (const auto &child : node.children)
    {
      oss << xmlToJson(child, indent + 4) << ",\n";
    }
    oss.seekp(-2, ios_base::end); // Remove trailing comma
    oss << "\n"
        << indentation << "  ]\n";
  }
  else
  {
    oss.seekp(-2, ios_base::end); // Remove trailing comma
    oss << "\n";
  }

  oss << indentation << "}";
  return oss.str();
}

// Function to convert XML to JSON and save the result to a file
void convertXmlToJson(const string &inputFile, const string &outputFile)
{
  // Read XML from file
  ifstream xmlFile(inputFile);
  if (!xmlFile)
  {
    cerr << "Error: Failed to open XML file: " << inputFile << endl;
    return;
  }

  // Parse the XML file into a tree structure
  XmlNode root = parseXml(xmlFile);
  xmlFile.close();

  // Convert the XML tree into JSON format
  string json = xmlToJson(root, 0);

  // Write the JSON to the output file
  ofstream jsonFile(outputFile);
  if (!jsonFile)
  {
    cerr << "Error: Failed to create JSON file: " << outputFile << endl;
    return;
  }

  jsonFile << json;
  jsonFile.close();

  cout << "Conversion completed successfully! \nFile saved to " << (string)outputFile << endl;
}
