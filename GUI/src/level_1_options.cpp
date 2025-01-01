#include "level_1_options.h"
#include "ui_level_1_options.h"

Level_1_Options::Level_1_Options(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Level_1_Options)
{
    ui->setupUi(this);
}

Level_1_Options::~Level_1_Options()
{
    delete ui;
}
