#include "settingsForm.h"
#include "ui_settings.h"
#include <QDataStream>
#include <QBuffer>
#include <QTableWidgetItem>

SettingsForm::SettingsForm(QWidget *parent) : QWidget(parent),
        ui(new Ui::SettingsForm)

{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint |
                Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    connect(ui->okPshBttn, SIGNAL(clicked(bool)), this, SIGNAL(okBttnClick(bool)),
            Qt::DirectConnection);
    ui->crcTypecmbBx->clear();
    ui->inputFormatcmbBx->clear();
    ui->outputNumbercmbBx->clear();
    ui->langCmbBx->clear();
    //ui->crcTypecmbBx->addItems({"CRC16", "CRC32", "CRC8", "CRC16 MODBUS", "vegaCrc32"});
    ui->inputFormatcmbBx->addItems({"Hex", "C++ стиль"});
    ui->outputNumbercmbBx->addItems({"Hex", "Dec", "Hex(C++ стиль)"});
    ui->langCmbBx->addItems({"Русский", "English"});
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void SettingsForm::setSettings(const QMap<QString, QVariant> &settings)
{
    if(settings.contains("inputFormat"))
        ui->inputFormatcmbBx->setCurrentText(settings["inputFormat"].toString());
    if(settings.contains("outputFormat"))
        ui->outputNumbercmbBx->setCurrentText(settings["outputFormat"].toString());
    if(settings.contains("language"))
        ui->langCmbBx->setCurrentText(settings["language"].toString());
    if(settings.contains("delimeter"))
        ui->delimeterlineEdit->setText(settings["delimeter"].toString());
    // Таблица типов CRC
    if(settings.contains("crcTypes")) {
        QBuffer buf;
        QByteArray arr = settings["crcTypes"].toByteArray();
        buf.setBuffer(&arr);
        QDataStream ds(&buf);
        ds >> crcTypes;
    }
    else crcTypes.clear();
    updateCrcTypeTable();
    // Заполняем комбобокс наименованиями типов CRC
    for(auto &item: crcTypes) {
        if(verifyCrctype(item)) {
            ui->crcTypecmbBx->addItem(item[0].toString());
        }
    }
    if(settings.contains("crcType"))
        ui->crcTypecmbBx->setCurrentText(settings["crcType"].toString());

}


bool SettingsForm::verifyCrctype(QVector<QVariant> &crcType)
{
    if(crcType.size() < 4) return false;
    bool ok;
    if(crcType[0].toString() == "") return false; // Наименование не должно быть пустым
    QString::number(crcType[1].toUInt(&ok));
    if(!ok) return false;
    QString::number(crcType[2].toUInt(&ok));
    if(!ok) return false;
    uint64_t polynome = crcType[3].toUInt(&ok);
    if(!ok) return false;
    if(polynome == 0) return false; // Полином не должен быть нулевым.
    return true;
}



void SettingsForm::getSettings(QMap<QString, QVariant> &settings) const
{
    settings["crcType"] = ui->crcTypecmbBx->currentText();
    settings["inputFormat"] = ui->inputFormatcmbBx->currentText();
    settings["outputFormat"] = ui->outputNumbercmbBx->currentText();
    settings["language"] = ui->langCmbBx->currentText();
    settings["delimeter"] = ui->delimeterlineEdit->text();
    QBuffer buf;
    QDataStream ds(&buf);
    ds << crcTypes;
    settings["crcTypes"] = buf.buffer();
}

void SettingsForm::updateCrcTypeTable()
{
    ui->tableWidget->setRowCount(crcTypes.size());
    for(uint16_t i=0; i<ui->tableWidget->rowCount(); i++) {
        for(uint16_t j=0; j<ui->tableWidget->columnCount(); j++) {
            if(ui->tableWidget->item(i, j) == nullptr) {
                QTableWidgetItem *item = new QTableWidgetItem("");
                ui->tableWidget->setItem(i, j, item);
            }
            ui->tableWidget->item(i, j)->setData(Qt::DisplayRole, crcTypes[i][j]);
        }
    }
}

/*void SettingsForm::updateCrcTypeArray()
{
    crcTypes.resize(ui->tableWidget->rowCount());
    for(uint16_t i=0; i<ui->tableWidget->rowCount(); i++) {
        for(j=0; j<ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->item(i, j)->setData(Qt::DisplayRole, crcTypes[i][j]);
        }
    }
}*/


void SettingsForm::on_addCrcTypeBttn_clicked()
{
    crcTypes.resize(crcTypes.size()+1);
    crcTypes[crcTypes.size()-1].resize(ui->tableWidget->columnCount());
    for(auto &item : crcTypes[crcTypes.size()-1]) {
        item = QVariant("");
    }
    updateCrcTypeTable();
}




void SettingsForm::on_removeCrcTypeBttn_clicked()
{
    if(crcTypes.size()) {
        crcTypes.resize(crcTypes.size()-1);
        updateCrcTypeTable();
    }
}
