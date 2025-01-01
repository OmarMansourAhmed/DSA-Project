#ifndef QTEXTEDIT_H
#define QTEXTEDIT_H

#include <QDialog>

namespace Ui {
class QTextEdit;
}

class QTextEdit : public QDialog
{
    Q_OBJECT

public:
    explicit QTextEdit(QWidget *parent = nullptr);
    ~QTextEdit();

private:
    Ui::QTextEdit *ui;
};

#endif // QTEXTEDIT_H
