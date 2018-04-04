#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QVector>
#include <QVariant>

namespace Ui {
class SettingsForm;
}



class SettingsForm : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsForm(QWidget *parent = nullptr);
    void setSettings(const QMap<QString, QVariant> &settings);
    void getSettings(QMap<QString, QVariant> &settings) const;
signals:
    void okBttnClick(bool);
public slots:

private slots:

    void on_addCrcTypeBttn_clicked();

    void on_removeCrcTypeBttn_clicked();

private:
    Ui::SettingsForm *ui;
    QVector<QVector<QVariant>> crcTypes;
    void updateCrcTypeTable();
    void updateCrcTypeArray();
    bool verifyCrctype(QVector<QVariant> &crcType);
};

#endif // SETTINGS_H
