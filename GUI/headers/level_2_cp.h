#ifndef LEVEL_2_CP_H
#define LEVEL_2_CP_H

#include <QDialog>

namespace Ui {
class level_2_CP;
}

class level_2_CP : public QDialog
{
    Q_OBJECT

public:
    explicit level_2_CP(QWidget *parent = nullptr);
    ~level_2_CP();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_17_clicked();

private:
    Ui::level_2_CP *ui;
};

#endif // LEVEL_2_CP_H
