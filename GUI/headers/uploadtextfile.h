/*#ifndef UPLOADTEXTFILE_H
#define UPLOADTEXTFILE_H

#include <QDialog>

namespace Ui {
class uploadTextFile;
}

class uploadTextFile : public QDialog
{
    Q_OBJECT

public:
    explicit uploadTextFile(QWidget *parent = nullptr);
    ~uploadTextFile();

private slots:
    void on_pushButton_clicked();

private:
    Ui::uploadTextFile *ui;
};

#endif // UPLOADTEXTFILE_H
*/

#ifndef UPLOADTEXTFILE_H
#define UPLOADTEXTFILE_H

#include <QDialog>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class uploadTextFile; }
QT_END_NAMESPACE

class uploadTextFile : public QDialog
{
    Q_OBJECT

public:
    explicit uploadTextFile(QWidget *parent = nullptr);
    ~uploadTextFile();

private slots:
    void on_pushButton_clicked();   // Prettifying
    void on_pushButton_2_clicked(); // Check XML consistency
    void on_pushButton_3_clicked(); // Slot for converting XML to JSON
    void on_pushButton_4_clicked(); // Minify XML
    void on_pushButton_5_clicked(); // Compress file
    void on_pushButton_6_clicked(); // DeCompress file
    void on_pushButton_7_clicked(); // Next Page


private:
    Ui::uploadTextFile *ui;
    void displayOutputFile(const QString &filePath); // New method to display output
};

#endif // UPLOADTEXTFILE_H
