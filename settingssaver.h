#ifndef SETTINGSSAVER_H
#define SETTINGSSAVER_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QString>
#include <QDataStream>

class SettingsSaver : public QObject
{
    Q_OBJECT
public:
    explicit SettingsSaver(QObject *parent = 0);
    explicit SettingsSaver(const QString &fileName, QObject *parent = 0);
    void setFileName(const QString &fileName) {_fileName = fileName; }
    void setSetting(const QString &paramName, const QVariant &value);
    bool getSetting(const QString &paramName, QVariant &value);
    void saveToFile(const QString &fileName="");
    bool loadFromFile(const QString &fileName="");
signals:

public slots:

private:
    static const int CODE1 = 0xFAC45183;
    static const int CODE2 = 0x7B451F01;
    QString _fileName;
    QHash<QString, QVariant> _settings;
};

#endif // SETTINGSSAVER_H
