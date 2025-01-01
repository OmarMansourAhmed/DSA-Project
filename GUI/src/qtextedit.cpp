#include "qtextedit.h"
#include "ui_qtextedit.h"

QTextEdit::QTextEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QTextEdit)
{
    ui->setupUi(this);
}

QTextEdit::~QTextEdit()
{
    delete ui;
}
