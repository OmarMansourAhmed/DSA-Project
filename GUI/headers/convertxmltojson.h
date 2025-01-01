#ifndef CONVERTXMLTOJSON_H
#define CONVERTXMLTOJSON_H

#include <string>

// Converts an XML file to a JSON file.
// Parameters:
// - inputFile: Path to the input XML file.
// - outputFile: Path to the output JSON file where the converted data will be saved.
void convertXmlToJson(const std::string &inputFile, const std::string &outputFile);

#endif // CONVERTXMLTOJSON_H
