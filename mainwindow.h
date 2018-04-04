#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QSharedPointer>
#include <QRegExpValidator>
#include "settingsForm.h"
#include "settingssaver.h"
#include "aboutdialog.h"


namespace Ui {
class MainWindow;
}

/*enum class MySettings_t : QString
{
    Language = "Language",
    CrcType = "CrcType"
};*/


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void calculate(bool);
    void on_settingsClicked();
    void onClickedSettingsOkbttn(bool);
    void onTextChanged();
    void onFileSaveAction(bool);
    void onFileLoadAction(bool);
    void onShowAbout(bool);
private:
    Ui::MainWindow *ui;
    AboutDialog *aboutDialog;
    //QSharedPointer<SettingsForm> sharedSettingsForm;
    SettingsForm *settingsForm;
    SettingsSaver *saver;
    static const QList<QString> Table;
    QMap <QString, QVariant> settings;
    static const QMap <QString, QVariant> DefaultSettings;
    static const QMap <QString, QString> regExprTypes;
    static const QString SETTINGS_FILE_NAME;
    void loadSettings();
    uint32_t calcCrc(const QByteArray &array, uint8_t &crcSize);
    void parsingData(const QString &inData, QByteArray &out);
    QRegExpValidator& getValidator();
    void makeOutText(const QByteArray &array, QString &out);
    void saveSettings();
};

#endif // MAINWINDOW_H
