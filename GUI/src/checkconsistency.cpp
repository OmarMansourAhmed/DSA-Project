#include "checkconsistency.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>
#include <algorithm>
#include <cctype>

using namespace std;

string Erase_Spaces(const string &str) {
    string result;
    for (char c : str) {
        if (!isspace(c))
            result += c;
    }
    return result;
}

bool isData(char c) {
    return !isspace(c) && c != '<' && c != '>';
}

vector<string> XML_Parsing_Data(const string &data) {
    bool Inside_Tag = false, Inside_Data = false, Outside_Tag = false, Outside_Data = false;
    vector<string> XML;
    string New_Str = "", Temp = "";

    for (unsigned int i = 0; i < data.length(); i++) {
        if (data[i] == '<') {
            Inside_Tag = true;
            Outside_Tag = false;
            Inside_Data = false;
            Outside_Data = false;
            if (New_Str != "") {
                XML.push_back(Erase_Spaces(New_Str));
                New_Str = "";
            }
        } else if (data[i] == '>') {
            Inside_Tag = false;
            Outside_Tag = true;
            Inside_Data = false;
            Outside_Data = false;
            New_Str += data[i];
            XML.push_back(New_Str);
            New_Str = "";
        }

        if (isData(data[i]) && !Inside_Tag) {
            Outside_Tag = false;
            Inside_Data = true;
            Outside_Data = false;
            New_Str += Temp;
            Temp = "";
        }

        if ((data[i] == ' ' || data[i] == '\n' || data[i] == '\t') && Inside_Data) {
            Inside_Tag = false;
            Inside_Data = false;
            Outside_Data = true;
            Temp += data[i];
        }

        if (Inside_Tag) {
            New_Str += data[i];
        }

        if (Inside_Data && !Outside_Tag) {
            New_Str += data[i];
        }
    }

    return XML;
}

vector<string> Parse_XML_File(const string &file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_path << endl;
        return {};
    }

    string data = "", line;
    while (getline(file, line)) {
        data += line + "\n";
    }

    file.close();
    return XML_Parsing_Data(data);
}

string Get_Tag(string Open_Tag) {
    string open = "";
    for (unsigned int i = 1; i < Open_Tag.length(); i++) {
        while (Open_Tag[i] != ' ' && Open_Tag[i] != '>') {
            open += Open_Tag[i++];
        }
        break;
    }
    return open;
}

bool Compare_Tags(string Open_Tag, string Close_Tag) {
    string open = Get_Tag(Open_Tag);
    return (Erase_Spaces(open) == Erase_Spaces(Close_Tag.substr(2, Close_Tag.length() - 3)));
}

char Get_Type(string str)
{
    if (str[0] == '<')
    {
        if (str[1] == '/')
            return 'C';
        else if (str[1] == '!' && str[2] == '-' && str[3] == '-')
            return 'C';
        return 'O';
    }
    return 'V';
}

int checking_errors(vector<string> &strr, vector<string> &modified, vector<int> &error) {
    int Error_count = 0;
    stack<string> s;

    for (unsigned int i = 0; i < strr.size(); i++) {
        if (strr[i][0] != '<') {
            modified.push_back(strr[i]);
            continue;
        }

        if ((strr[i][0] == '<' && strr[i][1] == '!') || (strr[i][0] == '<' && strr[i][1] == '?')) {
            modified.push_back(strr[i]);
            continue;
        }

        if (strr[i][0] == '<' && strr[i][1] != '/' && strr[i][1] != '!') {
            if (!s.empty() && strr[i - 1][0] != '<') {
                error.push_back(i - 1);
                Error_count++;
                string nn = s.top();
                nn.insert(1, "/");
                modified.push_back(nn);
                s.pop();
            }
            s.push(strr[i]);
            modified.push_back(strr[i]);
        } else if (strr[i][0] == '<' && strr[i][1] == '/') {
            if (!s.empty()) {
                string er = s.top();
                if (Compare_Tags(er, strr[i])) {
                    s.pop();
                    modified.push_back(strr[i]);
                } else {
                    string app = er.insert(1, "/");
                    modified.push_back(app);
                    s.pop();
                    error.push_back(i);
                    Error_count++;
                }
            } else {
                error.push_back(i);
                Error_count++;
                modified.push_back(strr[i]);
            }
        }
    }

    while (!s.empty()) {
        string m = s.top();
        m.insert(1, "/");
        modified.push_back(m);
        s.pop();
    }

    return Error_count;
}

void Write_XML_File(const string &file_path, const vector<string> &tokenized_data) {
    ofstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_path << " for writing." << endl;
        return;
    }

    for (const string &element : tokenized_data) {
        file << element << endl;
    }

    file.close();
}
