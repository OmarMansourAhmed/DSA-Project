#include "displaycontent.h"
#include "ui_displaycontent.h"
/*
displayContent::displayContent(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::displayContent)
{
    ui->setupUi(this);
}

displayContent::~displayContent()
{
    delete ui;
}

#include "displayContent.h"
#include "ui_displayContent.h"
*/
displayContent::displayContent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displayContent)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true); // Ensure the text edit is read-only
}

displayContent::~displayContent()
{
    delete ui;
}

void displayContent::setText(const QString &content)
{
    ui->textEdit->setText(content);
}
