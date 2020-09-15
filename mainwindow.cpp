#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Open the desired file
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), desktopPath, tr("JSON(*.json);;Config(*.config);;ini(*.ini);;Text(*.txt);;All File(*)"));
    QFile file(fileName);
    currentFile = fileName;

    // Check for IO Errors
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot Open File: " + file.errorString());
        return;
    }

    // Read lines into a list
    QTextStream in(&file);
    QVector<QString> rawParametersList;
    QMap<QString, QString> parametersMap;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line[0] == '[') continue;
        rawParametersList.push_back(line);
    }

    // Close the list
    file.close();

    // Split the line list into parameters and put them in a map
    QString splitter = "=";

    for (auto parameter : rawParametersList) {
        QString key;
        QString value;

        QStringList split = parameter.split(splitter);
        key = split.value(0);
        value = split.value(1);

        parametersMap.insert(key, value);
    }

    parameterBoxesMap.clear();

    // Add Found Parameters to the layout
    for (auto key : parametersMap.keys()) {
        auto settingsBox = CreateSettingLabel(key, parametersMap[key]);
        ui->verticalLayout->addLayout(settingsBox);
    }

    ApplicationInfo* appInfo = new ApplicationInfo("Random", "C:/Users/Carolina/Desktop/Glamorg_Logo.png", "Test");
    ui->label->setPixmap(appInfo->GetIcon().scaled(130, 130, Qt::KeepAspectRatio));
    ui->verticalLayout->addStretch();
}

QHBoxLayout* MainWindow::CreateSettingLabel(QString settingName, QString settingValue) {
    // Setup label
    QLabel* label = new QLabel(settingName);
    label->setMinimumWidth(100);

    // Setup line edit
    QLineEdit* lineEdit = new QLineEdit;
    lineEdit->setText(settingValue);
    label->setBuddy(lineEdit);

    // Add it to the map to be tracked
    parameterBoxesMap.insert(lineEdit, settingName);

    // Put them together in a layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(lineEdit);

    return layout;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_saveButton_clicked()
{
    // Open current file
    QFile file(currentFile);

    // Check for IO Errors
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot Open File: " + file.errorString());
        return;
    }

    // Setup the text stream
    QTextStream out(&file);
    QString text = "";

    // Write to the file
    for (auto parameter : parameterBoxesMap.keys()) {
        text += parameterBoxesMap[parameter] + "=" + parameter->text() + "\n";
    }

    out << text;
    file.close();
}

void MainWindow::on_saveAsButton_clicked()
{
    // Open current file
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("Text(*.txt);;All File(*)"));
    if (fileName.isEmpty()) return;
    QFile file(fileName);

    // Check for IO Errors
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot Open File: " + file.errorString());
        return;
    }

    // Setup the text stream
    QTextStream out(&file);
    QString text = "";

    // Write to the file
    for (auto parameter : parameterBoxesMap.keys()) {
        text += parameterBoxesMap[parameter] + "=" + parameter->text() + "\n";
    }

    out << text;
    file.close();
}
