#ifndef LEVEL_1_OPTIONS_H
#define LEVEL_1_OPTIONS_H

#include <QDialog>

namespace Ui {
class Level_1_Options;
}

class Level_1_Options : public QDialog
{
    Q_OBJECT

public:
    explicit Level_1_Options(QWidget *parent = nullptr);
    ~Level_1_Options();

private:
    Ui::Level_1_Options *ui;
};

#endif // LEVEL_1_OPTIONS_H
