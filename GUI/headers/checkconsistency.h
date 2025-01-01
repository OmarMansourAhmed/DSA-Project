#ifndef CHECKCONSISTENCY_H
#define CHECKCONSISTENCY_H

#include <vector>
#include <string>

using namespace std;

vector<string> Parse_XML_File(const string &file_path);
int checking_errors(vector<string> &strr, vector<string> &modified, vector<int> &error);
void Write_XML_File(const string &file_path, const vector<string> &tokenized_data);
char Get_Type(string str);
string Get_Tag(string Open_Tag);
bool Compare_Tags(string Open_Tag, string Close_Tag);
string Erase_Spaces(const string &str);
vector<string> XML_Parsing_Data(const string &data);

#endif // CHECKCONSISTENCY_H
