#include "outputtextedit.h"
#include "ui_outputtextedit.h"

outputTextEdit::outputTextEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::outputTextEdit)
{
    ui->setupUi(this);
}

outputTextEdit::~outputTextEdit()
{
    delete ui;
}
