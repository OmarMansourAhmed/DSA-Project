#ifndef OUTPUTTEXTEDIT_H
#define OUTPUTTEXTEDIT_H

#include <QDialog>

namespace Ui {
class outputTextEdit;
}

class outputTextEdit : public QDialog
{
    Q_OBJECT

public:
    explicit outputTextEdit(QWidget *parent = nullptr);
    ~outputTextEdit();

private slots:
    void on_textEdit_textChanged();

private:
    Ui::outputTextEdit *ui;
};

#endif // OUTPUTTEXTEDIT_H
