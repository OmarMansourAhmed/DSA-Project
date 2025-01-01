#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "copyandpaste.h"
#include "uploadtextfile.h"

secondWindow::secondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::secondWindow)
{
    ui->setupUi(this);
}

secondWindow::~secondWindow()
{
    delete ui;
}

void secondWindow::on_pushButton_clicked()
{
    if(ui->radioButton_2->isChecked()){
        hide();
        copyAndPaste copyPaste;
        copyPaste.setModal(true);
        copyPaste.exec();
    }else if (ui->radioButton->isChecked()){
        hide();
        uploadTextFile uploadFile;
        uploadFile.setModal(true);
        uploadFile.exec();
    }
}

