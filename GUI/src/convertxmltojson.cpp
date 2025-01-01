#include "convertXmlToJson.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

// Helper function to trim whitespace from a string
std::string trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Structure to represent an XML node
struct XmlNode {
    std::string name;                     // Tag name
    std::map<std::string, std::string> attributes; // Tag attributes
    std::string text;                     // Inner text
    std::vector<XmlNode> children;        // Child nodes
};

// Function to parse an XML input stream into an XmlNode tree
XmlNode parseXml(std::istream &input) {
    std::string line, tag;
    XmlNode root;                // Root node of the XML
    std::vector<XmlNode *> nodeStack; // Stack to keep track of nested nodes

    while (std::getline(input, line)) {
        line = trim(line); // Remove leading/trailing whitespace
        if (line.empty()) continue;

        if (line[0] == '<' && line[1] != '/') { // Start tag
            size_t tagEnd = line.find('>');
            tag = line.substr(1, tagEnd - 1);

            XmlNode node;
            size_t spacePos = tag.find(' ');

            // Parse tag name and attributes
            if (spacePos != std::string::npos) {
                node.name = tag.substr(0, spacePos);
                std::string attributesStr = tag.substr(spacePos + 1);
                size_t eq, quote1, quote2;

                while ((eq = attributesStr.find('=')) != std::string::npos) {
                    quote1 = attributesStr.find('"', eq);
                    quote2 = attributesStr.find('"', quote1 + 1);
                    std::string attrName = trim(attributesStr.substr(0, eq));
                    std::string attrValue = attributesStr.substr(quote1 + 1, quote2 - quote1 - 1);
                    node.attributes[attrName] = attrValue;
                    attributesStr = attributesStr.substr(quote2 + 1);
                }
            } else {
                node.name = tag; // No attributes
            }

            // Attach the node to its parent or set it as root
            if (!nodeStack.empty()) {
                nodeStack.back()->children.push_back(node);
                nodeStack.push_back(&nodeStack.back()->children.back());
            } else {
                root = node;
                nodeStack.push_back(&root);
            }
        } else if (line[0] == '<' && line[1] == '/') { // End tag
            nodeStack.pop_back();
        } else { // Text node
            if (!nodeStack.empty()) {
                nodeStack.back()->text += line;
            }
        }
    }

    return root;
}

// Function to convert an XmlNode tree into JSON format
std::string xmlToJson(const XmlNode &node, int indent = 0) {
    std::ostringstream oss;
    std::string indentation(indent, ' ');

    oss << "{\n";
    oss << indentation << "  \"name\": \"" << node.name << "\",\n";

    // Add attributes if present
    if (!node.attributes.empty()) {
        oss << indentation << "  \"attributes\": {\n";
        for (const auto &[key, value] : node.attributes) {
            oss << indentation << "    \"" << key << "\": \"" << value << "\",\n";
        }
        oss.seekp(-2, std::ios_base::end); // Remove trailing comma
        oss << "\n" << indentation << "  },\n";
    }

    // Add text if present
    if (!node.text.empty()) {
        oss << indentation << "  \"text\": \"" << trim(node.text) << "\",\n";
    }

    // Add children if present
    if (!node.children.empty()) {
        oss << indentation << "  \"children\": [\n";
        for (const auto &child : node.children) {
            oss << xmlToJson(child, indent + 4) << ",\n";
        }
        oss.seekp(-2, std::ios_base::end); // Remove trailing comma
        oss << "\n" << indentation << "  ]\n";
    } else {
        oss.seekp(-2, std::ios_base::end); // Remove trailing comma
        oss << "\n";
    }

    oss << indentation << "}";
    return oss.str();
}

// Implementation of convertXmlToJson function
void convertXmlToJson(const std::string &inputFile, const std::string &outputFile) {
    // Read XML from file
    std::ifstream xmlFile(inputFile);
    if (!xmlFile) {
        throw std::runtime_error("Failed to open XML file: " + inputFile);
    }

    // Parse the XML file into a tree structure
    XmlNode root = parseXml(xmlFile);
    xmlFile.close();

    // Convert the XML tree into JSON format
    std::string json = xmlToJson(root);

    // Write the JSON to the output file
    std::ofstream jsonFile(outputFile);
    if (!jsonFile) {
        throw std::runtime_error("Failed to create JSON file: " + outputFile);
    }

    jsonFile << json;
    jsonFile.close();
}
