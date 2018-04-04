#include "settingssaver.h"
#include <QFile>

SettingsSaver::SettingsSaver(QObject *parent) : QObject(parent)
{

}

SettingsSaver::SettingsSaver(const QString &fileName, QObject *parent):
        QObject(parent)
{
    _fileName = fileName;
}

// Возвращает true - в случае успеха
bool SettingsSaver::getSetting(const QString &paramName,
        QVariant &value)
{
    if(_settings.contains(paramName)) {
        value = _settings[paramName];
        return true;
    }
    else return false;
}

void SettingsSaver::saveToFile(const QString &fileName)
{
    QString fName;
    if(fileName == "") fName = _fileName;
    else fName = fileName;
    QFile settings(fName);
    settings.open(QIODevice::ReadWrite);
    QDataStream stream(&settings);
    stream << CODE1; // Признак файла настроек
    stream << CODE2;
    stream << _settings;

#warning Would be very good in future add CRC verify

    //volatile uint16_t port = ui->spinBox_port->value();
    //stream << ui->lineEdit_address->text();
    //stream << port;
    //    settings->write((const char*)&addr, sizeof(addr));
    //    settings->resize(settings->pos());
    settings.close();
}


// Возвращает true - в случае успеха
bool SettingsSaver::loadFromFile(const QString &fileName)
{
    QString fName;
    if(fileName == "") fName = _fileName;
    else fName = fileName;
    QFile file(fName);
    file.open(QIODevice::ReadWrite);
    if(file.size())
    {
        QDataStream stream(&file);
        int code1;
        int code2;
        stream >> code1;
        stream >> code2;
        if(code1 != CODE1 || code2 != CODE2) {
            file.close();
            _settings.clear();
            return false;
        }
        stream >> _settings;
        // QByteArray filedata = settings->read(100);
        //TServerAddress addr;

        //stream >> addr.ip;
        //stream >> addr.port;
        //ui->lineEdit_address->setText(addr.ip);
        //ui->spinBox_port->setValue(addr.port);
        return true;
    }
    else {
        file.close();
        _settings.clear();
        return false;
    }
}

void SettingsSaver::setSetting(const QString &paramName,
        const QVariant &value)
{
    _settings[paramName] = value;
}

