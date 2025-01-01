#include "copyandpaste.h"
#include "ui_copyandpaste.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "checkconsistency.h"
#include "displaycontent.h"
#include "prettifyXML.h"
#include "convertxmltojson.h"
#include "minifying.h"
#include "compression.h"
#include "level_2_cp.h"


using namespace std;

copyAndPaste::copyAndPaste(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copyAndPaste)
{
    ui->setupUi(this);
}

copyAndPaste::~copyAndPaste()
{
    delete ui;
}

void copyAndPaste::displayOutputFileContent(const QString &filePath)
{
    // Open the file to read its content
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open the file for display: " + filePath);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll(); // Read the entire file content
    file.close();

    // Create and show the displayContent window
    displayContent *display = new displayContent(this);
    display->setText(content); // Pass the content to the text edit
    display->exec(); // Show the dialog modally
}


void copyAndPaste::on_pushButton_clicked()
{
    // Get the text input from plainTextEdit
    QString textInput = ui->plainTextEdit->toPlainText();

    // Specify the initial file path where the text will be saved
    QString initialFilePath = "D:/Electrical/Senior-1/Data Structures and Algorithms/Project/CopyAndPaste/Parsing.xml";

    // Create and save the file
    QFile file(initialFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textInput; // Write the text to the file
        file.close(); // Close the file
    } else {
        QMessageBox::warning(this, "Error", "Failed to save the file at: " + initialFilePath);
        return;
    }

    // Perform the consistency check on the saved file
    string inputFilePath = initialFilePath.toStdString();
    vector<string> parsedData = Parse_XML_File(inputFilePath);

    if (parsedData.empty()) {
        QMessageBox::warning(this, "Error", "Failed to parse the XML file.");
        return;
    }

    vector<string> modifiedData;
    vector<int> errorIndices;
    int errorCount = checking_errors(parsedData, modifiedData, errorIndices);

    // Ask the user to select a directory or file path to save the modified output
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save Modified XML File", "", "XML Files (*.xml);;All Files (*)");

    if (saveFilePath.isEmpty()) {
        QMessageBox::information(this, "Info", "No save location selected. Operation cancelled.");
        return;
    }

    // Save the modified XML file
    Write_XML_File(saveFilePath.toStdString(), modifiedData);

    // Notify the user of the results
    QMessageBox::information(this, "Consistency Check Completed",
                             QString("Consistency check completed.\nErrors found: %1\nModified XML saved at: %2")
                                 .arg(errorCount)
                                 .arg(saveFilePath));
    // Display the content of the modified file in the displayContent window
    displayOutputFileContent(saveFilePath);
}


void copyAndPaste::on_pushButton_2_clicked()
{
    // Get the text input from plainTextEdit
    QString textInput = ui->plainTextEdit->toPlainText();

    // Specify the initial file path where the text will be saved
    QString initialFilePath = "D:/Electrical/Senior-1/Data Structures and Algorithms/Project/CopyAndPaste/input.xml";

    // Create and save the input file
    QFile file(initialFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textInput; // Write the text to the file
        file.close(); // Close the file
    } else {
        QMessageBox::warning(this, "Error", "Failed to save the file at: " + initialFilePath);
        return;
    }

    // Ask the user to select a directory or file path to save the prettified output
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save Prettified XML File", "", "XML Files (*.xml);;All Files (*)");

    if (saveFilePath.isEmpty()) {
        QMessageBox::information(this, "Info", "No save location selected. Operation cancelled.");
        return;
    }

    // Perform the prettifying operation
    prettifyXML(initialFilePath.toStdString(), saveFilePath.toStdString());

    // Notify the user of the results
    QMessageBox::information(this, "Prettify Completed", "XML prettified successfully and saved to: " + saveFilePath);

    // Display the prettified content in the displayContent window
    displayOutputFileContent(saveFilePath);
}


void copyAndPaste::on_pushButton_3_clicked()
{
    // Get the text input from plainTextEdit
    QString textInput = ui->plainTextEdit->toPlainText();

    // Specify the initial file path where the text will be saved as XML
    QString initialFilePath = "D:/Electrical/Senior-1/Data Structures and Algorithms/Project/CopyAndPaste/input.xml";

    // Create and save the input XML file
    QFile file(initialFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textInput; // Write the text to the file
        file.close(); // Close the file
    } else {
        QMessageBox::warning(this, "Error", "Failed to save the file at: " + initialFilePath);
        return;
    }

    // Ask the user to select a directory or file path to save the converted JSON file
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save JSON File", "", "JSON Files (*.json);;All Files (*)");

    if (saveFilePath.isEmpty()) {
        QMessageBox::information(this, "Info", "No save location selected. Operation cancelled.");
        return;
    }

    // Perform the XML to JSON conversion
    try {
        convertXmlToJson(initialFilePath.toStdString(), saveFilePath.toStdString());

        // Notify the user of the results
        QMessageBox::information(this, "Conversion Completed", "XML successfully converted to JSON and saved to: " + saveFilePath);

        // Display the JSON content in the displayContent window
        displayOutputFileContent(saveFilePath);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", QString("Conversion failed: ") + e.what());
    }
}


void copyAndPaste::on_pushButton_4_clicked()
{
    // Get the text input from plainTextEdit
    QString textInput = ui->plainTextEdit->toPlainText();

    // Specify the initial file path where the text will be saved as XML
    QString initialFilePath = "D:/Electrical/Senior-1/Data Structures and Algorithms/Project/CopyAndPaste/input.xml";

    // Create and save the input XML file
    QFile file(initialFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textInput; // Write the text to the file
        file.close(); // Close the file
    } else {
        QMessageBox::warning(this, "Error", "Failed to save the file at: " + initialFilePath);
        return;
    }

    // Ask the user to select a directory or file path to save the minified XML file
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save Minified XML File", "", "XML Files (*.xml);;All Files (*)");

    if (saveFilePath.isEmpty()) {
        QMessageBox::information(this, "Info", "No save location selected. Operation cancelled.");
        return;
    }

    // Perform the minifying operation
    try {
        minifying(initialFilePath.toStdString(), saveFilePath.toStdString());

        // Notify the user of the results
        QMessageBox::information(this, "Minifying Completed", "XML successfully minified and saved to: " + saveFilePath);

        // Display the minified content in the displayContent window
        displayOutputFileContent(saveFilePath);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", QString("Minifying failed: ") + e.what());
    }
}


void copyAndPaste::on_pushButton_5_clicked()
{
    // Get the text input from plainTextEdit
    QString textInput = ui->plainTextEdit->toPlainText();

    // Specify the initial file path where the text will be saved
    QString initialFilePath = "D:/Electrical/Senior-1/Data Structures and Algorithms/Project/CopyAndPaste/input.txt";

    // Create and save the input text file
    QFile file(initialFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << textInput; // Write the text to the file
        file.close(); // Close the file
    } else {
        QMessageBox::warning(this, "Error", "Failed to save the file at: " + initialFilePath);
        return;
    }

    // Ask the user to select a directory or file path to save the compressed output
    QString saveFilePath = QFileDialog::getSaveFileName(this, "Save Compressed File", "", "Text Files (*.txt);;All Files (*)");

    if (saveFilePath.isEmpty()) {
        QMessageBox::information(this, "Info", "No save location selected. Operation cancelled.");
        return;
    }

    // Perform the compression operation
    try {
        QFile inputFile(initialFilePath);
        if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Failed to open the input file for compression.");
            return;
        }

        QTextStream in(&inputFile);
        QString inputContent = in.readAll();
        inputFile.close();

        unordered_map<string, char> mapping;
        string compressedData = compress(inputContent.toStdString(), 10, mapping); // Perform compression

        QFile outputFile(saveFilePath);
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Failed to create the compressed file.");
            return;
        }

        QTextStream out(&outputFile);
        out << QString::fromStdString(compressedData); // Save compressed data
        outputFile.close();

        // Notify the user of the results
        QMessageBox::information(this, "Compression Completed", "File successfully compressed and saved to: " + saveFilePath);

        // Display the compressed content in the displayContent window
        // displayOutputFileContent(saveFilePath);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", QString("Compression failed: ") + e.what());
    }
}

void copyAndPaste::on_pushButton_7_clicked()
{
    hide();
    level_2_CP level_2;
    level_2.setModal(true);
    level_2.exec();
}

