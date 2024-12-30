# DSA-Project: XML Processing Tool

A comprehensive C++ tool for XML file processing, including parsing, validation, JSON conversion, minification, and compression/decompression capabilities.

## Features

- XML Parsing and Validation
- XML to JSON Conversion
- XML Minification
- File Compression
- File Decompression

## Prerequisites

- C++ Compiler (clang++ recommended)
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
│   └── prettifying_XML.cpp # XML prettifying implementation
├── headers/              # Header files directory
│   ├── Parse_XML.h      # XML parsing declarations
│   ├── ConvertToJSON.h  # JSON conversion declarations
│   ├── minifying.h      # XML minification declarations
│   ├── compression.h    # Compression declarations
│   ├── decompression.h  # Decompression declarations
│   └── prettifying_XML.h # XML prettifying declarations
└── README.md            # This file
```

## Building the Project

1. Open terminal in the project directory
2. Compile the project using clang++:
   ```bash
   clang++ -std=c++17 -I. main.cpp src/Parse_XML.cpp src/ConvertToJSON.cpp src/minifying.cpp src/compression.cpp src/decompression.cpp src/prettifying_XML.cpp -o xml_editor
   ```

## Running the Program

The XML editor can be used through command line with the following syntax:

```bash
./xml_editor <action> -i <input_file> [-o <output_file>]
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

- `minify`: Remove unnecessary whitespace

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

Note: All actions except `verify` require both input (-i) and output (-o) file parameters.

1. After compilation, run the program:

   ```bash
   ./xml_processor
   ```

2. You'll see a menu with the following options:

   - 1: XML Parsing and Validation
   - 2: XML to JSON Conversion
   - 3: XML Minification
   - 4: Compression
   - 5: Decompression
   - 6: Run All Tests
   - 0: Exit

3. Select an option by entering the corresponding number

## Input/Output Files

- Place your input XML files in the project directory
- Output files will be created in the same directory with appropriate extensions:
  - Parsed XML: `output_parsed.xml`
  - JSON conversion: `output.json`
  - Minified XML: `minified_output.xml`
  - Compressed file: `compressed_output.bin`
  - Decompressed file: `decompressed_output.xml`

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

## Sample Usage

1. Place your XML file as `sample.xml` in the project directory
2. Run the program and choose option 6 to test all features
3. Check the output files in the project directory

## Error Handling

- The program includes error checking for:
  - Invalid XML syntax
  - File read/write operations
  - Memory allocation
  - Invalid user input

## Contributing

Feel free to submit issues and enhancement requests!
