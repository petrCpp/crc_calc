#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QStringRef>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include "crc_lib/crc_lib.h"
#include <QTextDocument>
#include <Qfile>
#include <QFileDialog>
#include <QTextCodec>
#include <QTextStream>

const QString MainWindow::SETTINGS_FILE_NAME = "settings.cfg";

//const QList<QString> MainWindow::DefaultTable;

const QMap <QString, QVariant> MainWindow::DefaultSettings(
{
    {"language", QVariant("russian")},
    {"crcType", QVariant("vegaCrc32")},
    {"inputFormat", QVariant("Hex")},
    {"outputFormat", QVariant("Hex")},
    {"crcTypes", QVariant(QByteArray())}
});


const QMap <QString, QString> MainWindow::regExprTypes(
        {{"Hex", "^[\\s|,]{0,10}([0-9A-F]{1,2}[\\s|,]{1,10}){1,1000}"},
         {"C++ стиль", ""}});


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        settings(DefaultSettings)
{
    ui->setupUi(this);

    //sharedSettingsForm = QSharedPointer<SettingsForm>(new SettingsForm());
    //settingsForm = sharedSettingsForm.data();
    settingsForm = new SettingsForm(this);
    saver = new SettingsSaver(this);
    loadSettings();
    settingsForm->setSettings(settings);

    //connect(ui->calculateBttn, SIGNAL(clicked(bool)), this, SLOT(calculate(bool)),
    //        Qt::DirectConnection);

    connect(ui->settingsAction, SIGNAL(triggered(bool)), this,
            SLOT(on_settingsClicked()), Qt::DirectConnection);
    connect(settingsForm, SIGNAL(okBttnClick(bool)), this,
            SLOT(onClickedSettingsOkbttn(bool)), Qt::DirectConnection);

    connect(ui->inputTextEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()),
            Qt::DirectConnection);
    connect(ui->fileSaveAction, SIGNAL(triggered(bool)), this,
            SLOT(onFileSaveAction(bool)), Qt::DirectConnection);
    connect(ui->fileLoadAction, SIGNAL(triggered(bool)), this,
            SLOT(onFileLoadAction(bool)), Qt::DirectConnection);

    connect(ui->exitAction, SIGNAL(triggered(bool)), this, SLOT(close()),
            Qt::DirectConnection);

    connect(ui->AboutAction, SIGNAL(triggered(bool)), this, SLOT(onShowAbout(bool)),
            Qt::DirectConnection);
    aboutDialog = new AboutDialog(this);
    /*ui->typeCrcCmbBx->clear();
    ui->typeCrcCmbBx->addItem("Crc16");
    ui->typeCrcCmbBx->addItem("Crc32");

    ui->numbFormatCmbBx->clear();
    ui->numbFormatCmbBx->addItem("Hex");
    ui->numbFormatCmbBx->addItem("Dec");*/
}


void MainWindow::loadSettings()
{
    saver->loadFromFile("SETTINGS_FILE_NAME");
    QMap <QString, QVariant>::iterator item;
    for(item = settings.begin();item != settings.end(); item++) {
        QVariant value;
        if(saver->getSetting(item.key(), value)) {
            settings[item.key()] = value;
        }
    }
}

void MainWindow::saveSettings()
{
    QMap <QString, QVariant>::iterator item;
    for(item = settings.begin();item != settings.end(); item++) {
        saver->setSetting(item.key(), item.value());
    }
    saver->saveToFile("SETTINGS_FILE_NAME");
}



MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// Подсчитать контрольную сумму
uint32_t MainWindow::calcCrc(const QByteArray &array, uint8_t &crcSize)
{
    uint32_t crc = 0;
    crcSize = 0;
    if(settings["crcType"] == "CRC16") {
        crc = crc16_fast((uint8_t*)array.data(), array.size());
        crcSize = 2;
    }
    else if(settings["crcType"] == "CRC32") {
        crc = crc32_fast_full((uint8_t*)array.data(), array.size());
        crcSize = 4;
    }
    else if(settings["crcType"] == "vegaCrc32") {
        crc = crc32_fast_full((uint8_t*)array.data(), array.size());
        crcSize = 4;
    }
    else if(settings["crcType"] == "CRC8") {
        crc = crc8_fast((uint8_t*)array.data(), array.size());
        crcSize = 1;
    }
    else if(settings["crcType"] == "CRC16 MODBUS") {
        crc = crc16_modbus((uint8_t*)array.data(), array.size());
        crcSize = 2;
    }
    return crc;
}

QRegExpValidator& MainWindow::getValidator()
{

    QString str = "^[\\s|,]{0,10}([0-9A-F]{1,2}[\\s|,]{1,10}){1,1000}";

    if(settings["outputFormat"] == "Hex") {
        str = regExprTypes["Hex"];
    }
    else if(settings["outputFormat"] == "C++ стиль") {
        str = regExprTypes["C++ стиль"];
    }
    //if(ui->regExprlnEdt->text()!="") str = ui->regExprlnEdt->text();
    QRegExp hexReg(str);
    hexReg.setCaseSensitivity(Qt::CaseInsensitive);
    return *(new QRegExpValidator(hexReg, this));
}



void MainWindow::parsingData(const QString &inData, QByteArray &out)
{
    int index = 0;
    QString expr="";
    uint8_t base=10;
    if(settings["inputFormat"] == "Hex") {
        expr = "[0-9A-Fa-f]{1,2}";
        base = 16;
    }
    else if(settings["inputFormat"] == "C++ стиль") {
        expr= "((0[xX]){1,1}([0-9A-Fa-f]{1,2}))|([0-9]{1,3})";
        base = 10;
    }
    QRegularExpression regExpr(expr);
    regExpr.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch rmatch;
    int32_t value;
    QString str;
    out.clear();
    while(1) {
        index = inData.indexOf(regExpr, index, &rmatch);         // (reg, index, &match);
        if(index>=0) {
            bool ok;
            str = rmatch.captured();
            QStringRef subString(&str, 0, 2);
            if(subString == "0x") {
                 value = str.mid(2).toInt(&ok, 16);
            }
            else {
                value = str.toInt(&ok, base);
            }
            index += str.size();
            if(ok) out.append(static_cast<char>(value));
        }
        else break;
    }
}


void MainWindow::makeOutText(const QByteArray &array, QString &out)
{
    uint8_t base = 10;
    bool hexPrefix = false;
    if(settings["outputFormat"] == "Hex") {
        base = 16;
    }
    else if(settings["outputFormat"] == "Dec") {
        base = 10;
    }
    else if(settings["outputFormat"] == "Hex(C++ стиль)") {
        base = 16;
        hexPrefix = true;
    }
    QString delimeter = settings["delimeter"].toString();
    if(delimeter == "") delimeter = " ";
    for(int32_t i = 0; i<array.size(); i++) {
        if(hexPrefix) out += "0x";
        if(base == 16 && ((uint8_t)array[i]<0x10)) out += "0";
        out += QString(QString::number((uint8_t)array[i], base));
        out += delimeter;
    }
}


void MainWindow::calculate(bool)
{
    QByteArray out;
    QString outData;
    //ui->inputTextEdit->setValidator(getValidator());
    QString inData = ui->inputTextEdit->document()->toRawText();
    parsingData(inData, out);
    makeOutText(out, outData);
    QTextDocument *doc = new QTextDocument(outData);
    ui->outputTextEdit->setDocument(doc);
    uint8_t crcSize;
    uint32_t crc;
    crc = calcCrc(out, crcSize);
    uint64_t mult = 1, sum = 0;
    for(int32_t i=0; i<out.size(); i++) {
        mult*= static_cast<uint64_t>(static_cast<uint8_t>(out[i]));
        sum += static_cast<uint64_t>(static_cast<uint8_t>(out[i]));
    }

    uint8_t base = 10;
    QString hexPrefix = "";
    if(settings["outputFormat"] == "Hex") {
        base = 16;
    }
    else if(settings["outputFormat"] == "Dec") {
        base = 10;
    }
    else if(settings["outputFormat"] == "Hex(C++ стиль)") {
        hexPrefix = "0x";
        base = 16;
    }
    ui->crclineEdit->setText(hexPrefix+QString::number(crc, base));
    ui->sumLineEdit->setText(hexPrefix+QString::number(sum, base));
    ui->MultLineEdit->setText(hexPrefix+QString::number(mult, base));
}

void MainWindow::on_settingsClicked()
{
    settingsForm->setSettings(settings);
    settingsForm->show();
}

void MainWindow::onClickedSettingsOkbttn(bool)
{
    settingsForm->getSettings(settings);
    calculate(true);
    settingsForm->close();
}

void MainWindow::onTextChanged()
{
    calculate(true);
}

void MainWindow::onFileSaveAction(bool)
{
    //QFileDialog dialog();
    //dialog.exec();
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Сохранение текстового файла"), "",
            tr("Текстовые файлы (*.txt *.log)"));
    if(fileName == "") return;
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly| QFile::Truncate)) {
        QString str = ui->inputTextEdit->document()->toPlainText();
        QTextStream out(&file);
        out << str;
        file.close();
    }
}

void MainWindow::onFileLoadAction(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Открытие текстового файла"), "",
            tr("Текстовые файлы (*.txt *.log)"));
    if(fileName == "") return;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream out(&file);
    //int32_t maxLen = 32768;
    //QByteArray arr = file.read(maxLen);
    QString str;
    str = out.readAll();
    QTextDocument *docum = new QTextDocument(str);
    ui->inputTextEdit->setDocument(docum);
    file.close();
}

void MainWindow::onShowAbout(bool)
{
    aboutDialog->show();
}























