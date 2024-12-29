#ifndef PARSE_XML.H
#define PARSE_XML.H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <string>
#include <stack>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function to erase spaces from a string
string Erase_Spaces(const string &str);

// Function to check if a character is valid data
bool isData(char c);

// Function to parse XML data into a vector of strings
vector<string> XML_Parsing_Data(const string &data);

// Function to read the XML file and parse its content
vector<string> Parse_XML_File(const string &file_path);

// Function to get the type of a tag ('O' for opening, 'C' for closing, 'V' for value)
char Get_Type(string str);

// Function to extract the tag name from an opening tag
string Get_Tag(string Open_Tag);

// Function to compare opening and closing tags
bool Compare_Tags(string Open_Tag, string Close_Tag);

// Function to check and correct errors in XML tags
int checking_errors(vector<string> &strr, vector<string> &modified, vector<int> &error);

// Function to write corrected XML data to a file
void Write_XML_File(const string &file_path, const vector<string> &tokenized_data);

#endif // PARSE_XML.H
