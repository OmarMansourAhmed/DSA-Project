/*#ifndef DISPLAYCONTENT_H
#define DISPLAYCONTENT_H

#include <QDialog>

namespace Ui {
class displayContent;
}

class displayContent : public QDialog
{
    Q_OBJECT

public:
    explicit displayContent(QWidget *parent = nullptr);
    ~displayContent();

private:
    Ui::displayContent *ui;
};

#endif // DISPLAYCONTENT_H
*/
#ifndef DISPLAYCONTENT_H
#define DISPLAYCONTENT_H

#include <QDialog>

namespace Ui {
class displayContent;
}

class displayContent : public QDialog
{
    Q_OBJECT

public:
    explicit displayContent(QWidget *parent = nullptr);
    ~displayContent();

    void setText(const QString &content); // Method to set text in the text edit

private:
    Ui::displayContent *ui;
};

#endif // DISPLAYCONTENT_H
