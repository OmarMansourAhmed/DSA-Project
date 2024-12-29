#ifndef XML_TO_JSON_H
#define XML_TO_JSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

// Structure to represent an XML node
struct XmlNode
{
  string name;                    // Tag name
  map<string, string> attributes; // Tag attributes
  string text;                    // Inner text
  vector<XmlNode> children;       // Child nodes
};

// Helper function to trim whitespace from a string
string trim(const string &str);

// Function to parse an XML input stream into an XmlNode tree
XmlNode parseXml(istream &input);

// Function to convert an XmlNode tree into JSON format
string xmlToJson(const XmlNode &node, int indent = 0);

// Function to convert XML to JSON and save the result to a file
void convertXmlToJson(const string &inputFile, const string &outputFile);

#endif // XML_TO_JSON_H
