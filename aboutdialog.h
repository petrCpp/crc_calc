#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include <QDialog>
#include <QWidget>

namespace Ui {
    class AboutDialog;
}


class AboutDialog: public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = nullptr);
private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
// TODO: АБВ123ABC
