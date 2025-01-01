#ifndef COPYANDPASTE_H
#define COPYANDPASTE_H

#include <QDialog>

namespace Ui {
class copyAndPaste;
}

class copyAndPaste : public QDialog
{
    Q_OBJECT

public:
    explicit copyAndPaste(QWidget *parent = nullptr);
    ~copyAndPaste();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void on_pushButton_7_clicked();

private:
    Ui::copyAndPaste *ui;
    void displayOutputFileContent(const QString &filePath);
};

#endif // COPYANDPASTE_H
