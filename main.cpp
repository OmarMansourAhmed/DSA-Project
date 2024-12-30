#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "headers/Parse_XML.h"
#include "headers/ConvertToJSON.h"
#include "headers/minifying.h"
#include "headers/compression.h"
#include "headers/decompression.h"
#include "headers/prettifying_XML.h"
#include "headers/NetworkAnalysis.h"

using namespace std;



void printUsage()
{
    cout << "Usage: xml_editor <action> -i <input_file> [-f] [-o <output_file>] \n\n"
         << "Actions:\n"
         << "  verify        - Check XML file for errors\n"
         << "  prettify      - Format XML file with proper indentation\n"
         << "  minify        - Remove unnecessary whitespace\n"
         << "  compress      - Compress XML file\n"
         << "  decompress    - Decompress XML file\n"
         << "  to-json       - Convert XML to JSON format\n\n"
         << "Options:\n"
         << "  -i <file>     - Input file (required)\n"
         << "  -o <file>     - Output file (required for all actions except verify)\n"
         << "  -f            - Fix errors (only for verify action)\n";
}

string getOptionValue(int argc, char *argv[], const string &option)
{
    for (int i = 1; i < argc - 1; i++)
    {
        if (string(argv[i]) == option)
        {
            return argv[i + 1];
        }
    }
    return "";
}

bool hasFlag(int argc, char *argv[], const string &flag)
{
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == flag)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printUsage();
        return 1;
    }

    string action = argv[1];
    string inputFile = getOptionValue(argc, argv, "-i");
    string outputFile = getOptionValue(argc, argv, "-o");
    bool fixErrors = hasFlag(argc, argv, "-f");

    if (inputFile.empty())
    {
        cout << "Error: Input file is required\n";
        printUsage();
        return 1;
    }

    if (action != "verify" && outputFile.empty())
    {
        cout << "Error: Output file is required for action '" << action << "'\n";
        printUsage();
        return 1;
    }

    try
    {
        if (action == "verify")
        {
            vector<string> tokens = Parse_XML_File(inputFile);
            vector<string> correctedTags;
            vector<int> errorIndices;
            int error_count = checking_errors(tokens, correctedTags, errorIndices);

            if (error_count > 0)
            {
                cout << "Found " << error_count << " errors in XML file.\n";
                if (fixErrors && !outputFile.empty())
                {
                    cout << "Fixing errors and writing to " << outputFile << "...\n";
                    Write_XML_File(outputFile, correctedTags);
                    cout << "Fixed XML has been written to " << outputFile << endl;
                }
            }
            else
            {
                cout << "XML file is valid.\n";
            }
        }
        else if (action == "prettify")
        {
            prettifyXML(inputFile, outputFile);
        }
        else if (action == "minify")
        {
            minifying(inputFile, outputFile);
        }
        else if (action == "compress")
        {
            // Read input file
            ifstream inFile(inputFile);
            if (!inFile)
            {
                cout << "Error: Could not open input file\n";
                return 1;
            }
            stringstream buffer;
            buffer << inFile.rdbuf();
            string input = buffer.str();
            inFile.close();

            // Perform compression
            unordered_map<string, char> mapping;
            string compressed = compress(input, 100, mapping);

            // Write compressed data
            ofstream outFile(outputFile, ios::binary);
            if (!outFile)
            {
                cout << "Error: Could not create output file\n";
                return 1;
            }
            outFile << compressed;
            outFile.close();

            // Save mapping alongside the compressed file
            string mappingFile = outputFile + ".map";
            ofstream mapFile(mappingFile);
            if (!mapFile)
            {
                cout << "Error: Could not create mapping file\n";
                return 1;
            }
            // Write the number of mappings first
            size_t mapSize = mapping.size();
            mapFile << mapSize << "\n";

            // Write each mapping on a new line with explicit length
            for (const auto &pair : mapping)
            {
                mapFile << pair.first.length() << " " << pair.first << " " << (int)pair.second << "\n";
            }
            mapFile.close();
            cout << "Compression completed. Mapping saved to " << mappingFile << endl;
        }
        else if (action == "decompress")
        {
            cout << "Starting decompression process..." << endl;

            // Read compressed file
            ifstream inFile(inputFile, ios::binary);
            if (!inFile)
            {
                cout << "Error: Could not open compressed file\n";
                return 1;
            }
            cout << "Reading compressed file: " << inputFile << endl;
            stringstream buffer;
            buffer << inFile.rdbuf();
            string compressed = buffer.str();
            inFile.close();
            cout << "Compressed content size: " << compressed.size() << " bytes" << endl;

            // Read mapping from the mapping file (inputFile + .map)
            string mappingFile = inputFile + ".map";
            cout << "Reading mapping from: " << mappingFile << endl;
            unordered_map<string, char> mapping;
            ifstream mapFile(mappingFile);
            if (!mapFile)
            {
                cout << "Error: Could not find mapping file: " << mappingFile << "\n";
                return 1;
            }

            // Read number of mappings
            size_t mapSize;
            mapFile >> mapSize;
            mapFile.ignore(); // Skip newline
            cout << "Expected mappings: " << mapSize << endl;

            // Read each mapping
            for (size_t i = 0; i < mapSize; i++)
            {
                size_t keyLength;
                mapFile >> keyLength;
                mapFile.ignore(); // Skip space

                string key(keyLength, '\0');
                mapFile.read(&key[0], keyLength);

                int value;
                mapFile >> value;
                mapFile.ignore(); // Skip newline

                mapping[key] = (char)value;
                cout << "Loaded mapping " << (i + 1) << ": '" << key << "' -> '" << value << "'" << endl;
            }
            mapFile.close();
            cout << "Total mappings loaded: " << mapping.size() << endl;

            // Perform decompression
            cout << "Starting decompression..." << endl;
            string decompressed = decompress(compressed, mapping);
            cout << "Decompressed content size: " << decompressed.size() << " bytes" << endl;

            // Write decompressed data
            cout << "Writing decompressed data to: " << outputFile << endl;
            ofstream outFile(outputFile);
            if (!outFile)
            {
                cout << "Error: Could not create output file\n";
                return 1;
            }
            outFile << decompressed;
            outFile.close();
            cout << "Decompression completed successfully.\n";
        }
        else if (action == "to-json")
        {
            convertXmlToJson(inputFile, outputFile);
        }
        else
        {
            cout << "Error: Unknown action '" << action << "'\n";
            printUsage();
            return 1;
        }
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
