#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QVector>
#include <QStandardPaths>
#include <QSignalMapper>
#include <QDesktopServices>

#include "customparsers.h"
#include "applicationinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_saveButton_clicked();

    void on_saveAsButton_clicked();

    void SetupSettings(QString);

    void on_loadButton_clicked();

    void on_locationButton_clicked();

    void on_closeButton_clicked();

private:
    // VARIABLES
    Ui::MainWindow *ui;
    QString currentFile = "";
    ApplicationInfo currentApplication;
    QMap<QString, QLineEdit*> parameterBoxesMap;

    QVector<ApplicationInfo> defaultApps;

    QString projectPath;

    // FUNCTIONS

    void SetupAppOptions(QVector<ApplicationInfo>);

    QHBoxLayout* CreateSettingLabel(QString, QString);
    QPushButton* CreateAppOptionButton(ApplicationInfo);

    void ClearLayout(QLayout*);

    void BuildSettingsWidget(QString);
    bool CheckLoadFileValidity(QFile&);

    bool SetupProjectFolders();
};

#endif // MAINWINDOW_H
