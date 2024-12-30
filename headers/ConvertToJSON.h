#ifndef CONVERT_TO_JSON_H
#define CONVERT_TO_JSON_H

#include <string>
#include <map>
#include <vector>

struct XmlNode {
    std::string name;
    std::map<std::string, std::string> attributes;
    std::string text;
    std::vector<XmlNode> children;
};

XmlNode parseXml(std::istream &input);
std::string xmlToJson(const XmlNode &node, int indent = 0);
void convertXmlToJson(const std::string &inputFile, const std::string &outputFile);

#endif // CONVERT_TO_JSON_H
