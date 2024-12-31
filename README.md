# DSA-Project: XML Processing Tool

A comprehensive C++ tool for XML file processing, including parsing, validation, JSON conversion, minification, and compression/decompression capabilities.

## Features

- XML Parsing and Validation
- XML to JSON Conversion
- XML Minification
- File Compression
- File Decompression
- Network Analysis and Visualization

## Prerequisites

- C++ Compiler (clang++ recommended)
- Graphviz (required for graph visualization)
- Make sure you have read/write permissions in the project directory

## Project Structure

```
DSA-Project/
├── main.cpp              # Main program with test suite
├── src/                  # Source files directory
│   ├── Parse_XML.cpp     # XML parsing implementation
│   ├── ConvertToJSON.cpp # JSON conversion implementation
│   ├── minifying.cpp     # XML minification implementation
│   ├── compression.cpp   # Compression implementation
│   ├── decompression.cpp # Decompression implementation
│   ├── NetworkAnalysis/  # Network analysis implementations
│   │   ├── most_interactive.cpp
│   │   ├── mutualNetwork.cpp
│   │   ├── networkSearching.cpp
│   │   └── xmlInGraph.cpp
│   └── prettifying_XML.cpp # XML prettifying implementation
├── headers/              # Header files directory
│   ├── Parse_XML.h      # XML parsing declarations
│   ├── ConvertToJSON.h  # JSON conversion declarations
│   ├── minifying.h      # XML minification declarations
│   ├── compression.h    # Compression declarations
│   ├── decompression.h  # Decompression declarations
│   ├── NetworkAnalysis.h # Network analysis declarations
│   ├── mutualNetwork.h   # Mutual network declarations
│   ├── xmlInGraph.h      # Graph visualization declarations
│   └── prettifying_XML.h # XML prettifying declarations
└── README.md            # This file
```

## Building the Project

1. Open terminal in the project directory
2. Compile the project using clang++:
   ```bash
   clang++ -std=c++17 -I. main.cpp src/Parse_XML.cpp src/ConvertToJSON.cpp src/minifying.cpp src/compression.cpp src/decompression.cpp src/prettifying_XML.cpp src/NetworkAnalysis/most_interactive.cpp src/NetworkAnalysis/mutualNetwork.cpp src/NetworkAnalysis/networkSearching.cpp src/NetworkAnalysis/xmlInGraph.cpp -o xml_editor
   ```

## Running the Program

The XML editor can be used through command line with the following syntax:

```bash
./xml_editor <action> -i <input_file> [-o <output_file>] [-ids id1,id2,id3] [-id user_id] [-w word] [-t topic]
```

Available actions:

- `verify`: Check XML file for errors

  ```bash
  # Just verify the XML file
  ./xml_editor verify -i input.xml

  # Verify and fix errors, saving the corrected XML to output file
  ./xml_editor verify -i input.xml -f -o output.xml
  ```

- `prettify`: Format XML file with proper indentation

  ```bash
  ./xml_editor prettify -i input.xml -o output.xml
  ```

- `minify`: Remove unnecessary whitespace from XML

  ```bash
  ./xml_editor minify -i input.xml -o output.xml
  ```

- `compress`: Compress XML file

  ```bash
  ./xml_editor compress -i input.xml -o output.xml
  ```

- `decompress`: Decompress XML file

  ```bash
  ./xml_editor decompress -i input.xml -o output.xml
  ```

- `to-json`: Convert XML to JSON format

  ```bash
  ./xml_editor to-json -i input.xml -o output.json
  ```

- `most_active`: Find most active user in the network

  ```bash
  ./xml_editor most_active -i input.xml
  ```

- `most_influencer`: Find most influential user in the network

  ```bash
  ./xml_editor most_influencer -i input.xml
  ```

- `mutual`: Find mutual followers between users

  ```bash
  ./xml_editor mutual -i input.xml -ids "id1,id2,id3"
  ```

- `suggest`: Get user suggestions based on network

  ```bash
  ./xml_editor suggest -i input.xml -id user_id
  ```

- `search`: Search posts by word or topic

  ```bash
  ./xml_editor search -i input.xml -w word    # Search by word
  ./xml_editor search -i input.xml -t topic   # Search by topic
  ```

- `draw`: Generate network graph visualization

  ```bash
  ./xml_editor draw -i input.xml -o graph.jpg
  ```

Note: All actions except `verify` require both input (-i) and output (-o) file parameters.

## After compilation, run the program to print usage instructions:

```bash
./xml_processor
```

## Input/Output Files

- Place your input XML files in the project directory
- Output files will be created in the same directory with appropriate extensions:
  - Parsed XML: `output_parsed.xml`
  - JSON conversion: `output.json`
  - Minified XML: `minified_output.xml`
  - Compressed file: `compressed_output.bin`
  - Decompressed file: `decompressed_output.xml`
  - Network graph: `graph.jpg`

## Compression and Decompression

When compressing a file, two files will be created:

1. The compressed file (specified by `-o` parameter)
2. A mapping file with `.map` extension (automatically created alongside the compressed file)

For example:

```bash
# Compression
./xml_editor compress -i input.xml -o compressed.xml
# This will create:
# - compressed.xml (the compressed data)
# - compressed.xml.map (the mapping data)

# Decompression
./xml_editor decompress -i compressed.xml -o decompressed.xml
# This will automatically look for compressed.xml.map
```
