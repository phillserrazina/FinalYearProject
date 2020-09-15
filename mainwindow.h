#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QVector>
#include <QMap>
#include <QStandardPaths>

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

    QHBoxLayout* CreateSettingLabel(QString, QString);

private slots:
    void on_saveButton_clicked();

    void on_saveAsButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QMap<QLineEdit*, QString> parameterBoxesMap;
};

#endif // MAINWINDOW_H
