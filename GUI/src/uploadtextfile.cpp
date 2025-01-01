#include "uploadTextFile.h"
#include "ui_uploadTextFile.h"
#include "displayContent.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <iostream>
#include <string>
#include "prettifyXML.h"
#include "convertXmlToJson.h"
#include "compression.h"
#include "minifying.h"
#include "checkconsistency.h"
#include "decompression.h"
#include "level_2.h"

using namespace std;

uploadTextFile::uploadTextFile(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::uploadTextFile)
{
    ui->setupUi(this);
}

uploadTextFile::~uploadTextFile()
{
    delete ui;
}

// Utility function to save content to a file
void saveToFile(const std::string &filePath, const std::string &content) {
    QFile outFile(QString::fromStdString(filePath));
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Unable to create the output file: " + filePath);
    }
    outFile.write(QString::fromStdString(content).toUtf8());
    outFile.close();
}

// Utility function to display content in a dialog
void displayFileContent(QWidget *parent, const QString &content) {
    displayContent dialog(parent);
    dialog.setText(content);
    dialog.exec();
}

void uploadTextFile::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "XML Files (*.xml);;All Files (*.*)"
        );

    if (filename.isEmpty()) {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a file to proceed."));
        return;
    }

    std::string inputFile = filename.toStdString();
    std::string outputFile = inputFile + "_formatted.xml";

    try {
        prettifyXML(inputFile, outputFile);

        QFile file(QString::fromStdString(outputFile));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open the output file."));
            return;
        }
        QString fileContent = file.readAll();
        file.close();

        displayFileContent(this, fileContent);

        QMessageBox::information(
            this,
            tr("Success"),
            QString("XML file prettified successfully!\nOutput File: %1").arg(QString::fromStdString(outputFile))
            );
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), e.what());
    }
}

void uploadTextFile::on_pushButton_2_clicked() {
    // Step 1: Browse and select the XML file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open XML File"), "", tr("XML Files (*.xml);;All Files (*)"));
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!"));
        return;
    }

    // Step 2: Parse the XML file and validate it
    std::vector<std::string> parsedData = Parse_XML_File(filePath.toStdString());
    if (parsedData.empty()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to parse the XML file."));
        return;
    }

    std::vector<std::string> correctedTags;
    std::vector<int> errorIndices;

    int errorCount = checking_errors(parsedData, correctedTags, errorIndices);

    // Step 3: Prepare corrected XML content for display
    QString content;
    for (const std::string &line : correctedTags) {
        content += QString::fromStdString(line) + "\n";
    }

    // Display the corrected XML content
    displayFileContent(this, content);

    // Step 4: Automatically save the corrected XML to the same folder with a new name
    QFileInfo fileInfo(filePath);
    QString savePath = fileInfo.absolutePath() + "/corrected_" + fileInfo.fileName();

    Write_XML_File(savePath.toStdString(), correctedTags);

    // Inform the user about success
    QMessageBox::information(this, tr("Success"), tr("Corrected XML file saved successfully to:\n") + savePath);
}


void uploadTextFile::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Select XML File"),
        "C://",
        "XML Files (*.xml);;All Files (*.*)"
        );

    if (filename.isEmpty()) {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a file to proceed."));
        return;
    }

    std::string inputFile = filename.toStdString();
    std::string outputFile = inputFile + "_converted.json";

    try {
        convertXmlToJson(inputFile, outputFile);

        QFile file(QString::fromStdString(outputFile));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open the output JSON file."));
            return;
        }
        QString fileContent = file.readAll();
        file.close();

        displayFileContent(this, fileContent);

        QMessageBox::information(
            this,
            tr("Success"),
            QString("XML converted to JSON successfully!\nOutput File: %1").arg(QString::fromStdString(outputFile))
            );
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), e.what());
    }
}

void uploadTextFile::on_pushButton_4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Select File to Minify"),
        "C://",
        "XML Files (*.xml);;All Files (*.*)"
        );

    if (filename.isEmpty()) {
        QMessageBox::warning(this, tr("No File Selected"), tr("Please select a file to proceed."));
        return;
    }

    std::string inputFile = filename.toStdString();
    std::string outputFile = inputFile + "_minified.xml";

    try {
        minifying(inputFile, outputFile);

        QFile file(QString::fromStdString(outputFile));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Unable to open the minified file."));
            return;
        }
        QString fileContent = file.readAll();
        file.close();

        displayFileContent(this, fileContent);

        QMessageBox::information(
            this,
            tr("Success"),
            QString("File minified successfully!\nOutput File: %1").arg(QString::fromStdString(outputFile))
            );
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), e.what());
    }
}



void uploadTextFile::on_pushButton_5_clicked() {
    // Step 1: Browse and select the file to compress
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File to Compress"), "", tr("All Files (*.*)"));
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!"));
        return;
    }

    // Step 2: Read the file content
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) { // Use ReadOnly mode for binary data
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the file."));
        return;
    }

    QByteArray fileContent = file.readAll();
    file.close();

    std::string input = fileContent.toStdString();

    // Step 3: Compress the file content
    std::unordered_map<std::string, char> mapping;
    std::string compressedData = compress(input, 10, mapping);

    // Step 4: Save the compressed data to a file with a custom extension
    QFileInfo fileInfo(filePath);
    QString compressedFilePath = fileInfo.absolutePath() + "/compressed_" + fileInfo.baseName() + ".cmp"; // Custom extension

    QFile compressedFile(compressedFilePath);
    if (!compressedFile.open(QIODevice::WriteOnly)) { // Use WriteOnly mode for binary data
        QMessageBox::critical(this, tr("Error"), tr("Failed to save the compressed file."));
        return;
    }

    compressedFile.write(QByteArray::fromStdString(compressedData)); // Save binary data
    compressedFile.close();

    // Step 5: Save the mapping to a .map file
    QString mapFilePath = fileInfo.absolutePath() + "/compressed_" + fileInfo.baseName() + ".map";

    QFile mapFile(mapFilePath);
    if (!mapFile.open(QIODevice::WriteOnly | QIODevice::Text)) { // Save mapping as text
        QMessageBox::critical(this, tr("Error"), tr("Failed to save the mapping file."));
        return;
    }

    QTextStream mapStream(&mapFile);
    for (const auto &[pair, ch] : mapping) {
        mapStream << QString::fromStdString(pair) << " " << ch << "\n";
    }
    mapFile.close();

    // Step 6: Notify the user of success
    QMessageBox::information(this, tr("Success"),
                             tr("Compressed file saved to:\n%1\nMapping file saved to:\n%2")
                                 .arg(compressedFilePath)
                                 .arg(mapFilePath));
}


void uploadTextFile::on_pushButton_6_clicked() {
    // Step 1: Browse and select the compressed file
    QString compressedFilePath = QFileDialog::getOpenFileName(this, tr("Select Compressed File"), "", tr("All Files (*.*)"));
    if (compressedFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!"));
        return;
    }

    QFile compressedFile(compressedFilePath);
    if (!compressedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the compressed file."));
        return;
    }

    QByteArray compressedData = compressedFile.readAll();
    compressedFile.close();

    // Step 2: Browse and select the map file
    QString mapFilePath = QFileDialog::getOpenFileName(this, tr("Select Map File"), "", tr("Map Files (*.map);;All Files (*.*)"));
    if (mapFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No map file selected!"));
        return;
    }

    QFile mapFile(mapFilePath);
    if (!mapFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the map file."));
        return;
    }

    // Step 3: Read and parse the map file
    QTextStream mapStream(&mapFile);
    std::unordered_map<std::string, char> mapping;
    while (!mapStream.atEnd()) {
        QString line = mapStream.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() == 2) {
            mapping[parts[0].toStdString()] = parts[1].toStdString()[0];
        }
    }
    mapFile.close();

    if (mapping.empty()) {
        QMessageBox::critical(this, tr("Error"), tr("The map file is empty or invalid."));
        return;
    }

    // Step 4: Decompress the data
    std::string decompressedData;
    try {
        decompressedData = decompress(compressedData.toStdString(), mapping);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Error"), tr("Decompression failed: ") + QString::fromStdString(e.what()));
        return;
    }

    // Step 5: Save the decompressed data as an XML file
    QString savePath = QFileDialog::getSaveFileName(this, tr("Save Decompressed File"), "", tr("XML Files (*.xml)"));
    if (savePath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No save location selected!"));
        return;
    }

    QFile outputFile(savePath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to save the decompressed file."));
        return;
    }

    QTextStream out(&outputFile);
    out << QString::fromStdString(decompressedData);
    outputFile.close();

    // Step 6: Notify the user of success
    QMessageBox::information(this, tr("Success"), tr("Decompressed file saved successfully to:\n%1").arg(savePath));
}

void uploadTextFile::on_pushButton_7_clicked() {
    hide();
    Level_2 levelTwo;
    levelTwo.setModal(true);
    levelTwo.exec();
}
