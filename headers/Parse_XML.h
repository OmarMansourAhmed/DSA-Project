#ifndef PARSE_XML_H
#define PARSE_XML_H

#include <vector>
#include <string>

std::vector<std::string> XML_Parsing_Data(const std::string &data);
std::vector<std::string> Parse_XML_File(const std::string &file_path);
char Get_Type(std::string str);
std::string Get_Tag(std::string Open_Tag);
bool Compare_Tags(std::string Open_Tag, std::string Close_Tag);
int checking_errors(std::vector<std::string> &strr, std::vector<std::string> &modified, std::vector<int> &error);
void Write_XML_File(const std::string &file_path, const std::vector<std::string> &tokenized_data);

#endif // PARSE_XML_H
