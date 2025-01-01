#ifndef LEVEL_2_H
#define LEVEL_2_H

#include <QDialog>

namespace Ui {
class Level_2;
}

class Level_2 : public QDialog
{
    Q_OBJECT

public:
    explicit Level_2(QWidget *parent = nullptr);
    ~Level_2();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Level_2 *ui;
};

#endif // LEVEL_2_H
