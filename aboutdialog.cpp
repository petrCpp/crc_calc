#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog |
            Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint |
            Qt::CustomizeWindowHint);

    ui->plainTextEdit->setPlainText("Программа предназначена для "
            "для автоматического расчета циклических сверток CRC различных типов"
            ". \n\n\n\n"
            "Автор программы Абросимов Петр. \n"
            "Email: petruha_1@ngs.ru\n");
            QString progVers = "Версия программы: ";
            progVers += "1.0.0.0";
            progVers += "\n";
            progVers += "Дата сборки программы: "+ QString(__DATE__) + " в "  + QString(__TIME__);
            ui->plainTextEdit->appendPlainText(progVers);
}
