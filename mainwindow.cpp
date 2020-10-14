#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->settingsGroupBox->setVisible(false);

    SetupProjectFolders();

    defaultApps = {
        ApplicationInfo("Skyrim", projectPath + "/Icons/Skyrim_Logo.png", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/My Games/Skyrim Special Edition/Skyrim.ini"),
        ApplicationInfo("Witcher 3", projectPath + "/Icons/Witcher 3_Logo.png", "D:/QT Projects/ConfigurateProject//TestSaveFiles/witcherConfigTest.txt"),
        ApplicationInfo("Sun Rings", projectPath + "/Icons/Sun Rings_Logo.png" , "D:/QT Projects/ConfigurateProject//TestSaveFiles/sunRingsTest.json"),
        ApplicationInfo("Darkest Dungeon", projectPath + "/Icons/Darkest Dungeon_Logo.png", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Darkest/persist.options.json")
    };

    SetupAppOptions(defaultApps);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::SetupProjectFolders() {
    // Get app data path
    projectPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Rename the path
    QStringList projectSplit = projectPath.split("/");
    projectSplit.removeLast();
    projectPath = "";

    for (auto folder : projectSplit)
        projectPath += folder + "/";

    projectPath += "Configurate/";

    if (!QDir(projectPath).exists())
        QDir().mkdir(projectPath);

    // Necessary Resource Folders
    QStringList folders = { "Icons" };

    // Create folders if they don't exist
    for (auto folder : folders) {
        auto path = projectPath + folder;

        if (!QDir(path).exists())
            QDir().mkdir(path);
    }

    // Populate folders with standard resources
    QStringList defaultAppsNames = { "Skyrim", "Witcher 3", "Sun Rings", "Darkest Dungeon" };

    for (auto app : defaultAppsNames) {
        auto logoPath = app + "_Logo.png";
        QFile::copy(":/Resources/Images/" + logoPath, projectPath + "/Icons/" + logoPath);
    }


    return true;
}

// ========= FUNCTIONS ==========

void MainWindow::SetupAppOptions(QVector<ApplicationInfo> list) {
    for (auto app : list) {
        ui->appOptionsLayout->addWidget(CreateAppOptionButton(app));
    }

    ui->appOptionsLayout->addStretch();
}

void MainWindow::ClearLayout(QLayout* layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            ClearLayout(item->layout());
            delete item;
        }
        else if (item->widget()) {
            QWidget* widget;
            widget = item->widget();
            delete widget;
        }

    }
}

void MainWindow::SetupSettings(QString appName) {
    if (!ui->settingsGroupBox->isVisible()) ui->settingsGroupBox->setVisible(true);
    ui->settingsGroupBox->setTitle(appName);

    QString path = "";

    for (auto app : defaultApps) {
        if (app.GetName() == appName) {
            currentApplication = app;
            break;
        }
    }

    path = currentApplication.GetFilePath();
    BuildSettingsWidget(path);
}

void MainWindow::BuildSettingsWidget(QString filePath) {
    // Clear out current layout
    ClearLayout(ui->settingsLayout);

    // Map where the params will be stored
    QMap<QString, QString> parametersMap;

    // Check for file validity and parse it
    try {
        parametersMap = CustomParsers::Parse(filePath);
    }  catch (...) {
        if (ui->settingsGroupBox->isVisible()) ui->settingsGroupBox->setVisible(false);
        QMessageBox::warning(this, "Warning", "Unsupported File Type");
        currentFile = "";
        return;
    }

    currentFile = filePath;
    // Reset the Box-Parameter Link Map
    parameterBoxesMap.clear();

    // Add Found Parameters to the layout
    for (auto key : parametersMap.keys()) {
        auto settingsBox = CreateSettingLabel(key, parametersMap[key]);
        ui->settingsLayout->addLayout(settingsBox);
    }

    ui->settingsLayout->addStretch();
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
    parameterBoxesMap.insert(settingName, lineEdit);

    // Put them together in a layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    layout->addWidget(lineEdit);

    return layout;
}

QPushButton* MainWindow::CreateAppOptionButton(ApplicationInfo appInfo) {
    // Setup Button
    QPushButton* button = new QPushButton(" ");
    QIcon icon(appInfo.GetIcon());
    button->setIcon(icon);
    button->setIconSize(QSize(30, 30));
    button->setStyleSheet("text-align:left");
    button->setLayout(new QGridLayout);

    QLabel* textLabel = new QLabel(appInfo.GetName());
    QFont font("Arial", 10, QFont::Bold);
    textLabel->setFont(font);

    textLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter); // or center
    textLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    button->layout()->addWidget(textLabel);

    QSignalMapper* mapper = new QSignalMapper();
    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(SetupSettings(QString)));

    connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(button, appInfo.GetName());

    return button;
}

bool MainWindow::CheckLoadFileValidity(QFile& myFile) {

    // Read lines into a list
    QFile current(currentFile);

    if (!current.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot Open File: " + current.errorString());
        return false;
    }

    QTextStream in(&myFile);
    QTextStream inCurrent(&current);
    QVector<QString> rawParametersList;
    QVector<QString> currentParametersList;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line[0] == '[' || line.isEmpty()) continue;
        rawParametersList.push_back(line);
    }

    in.seek(0);

    while (!inCurrent.atEnd()) {
        QString line = inCurrent.readLine();
        if (line[0] == '[' || line.isEmpty()) continue;
        currentParametersList.push_back(line);
    }

    current.close();

    for (int i = 0; i < currentParametersList.size(); i++) {
        if (currentParametersList[i][0] != rawParametersList[i][0]) {
            return false;
        }

    }

    return true;
}

// ========= SLOTS ========

void MainWindow::on_saveButton_clicked() {
    CustomParsers::Save(currentFile, parameterBoxesMap);
}

void MainWindow::on_saveAsButton_clicked() {

}

void MainWindow::on_loadButton_clicked()
{
    // Open desired file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("ini(*.ini);;json(*.json);;All File(*)"));
    if (fileName.isEmpty()) return;

    // TODO: File match checks here
    QFile file(fileName);

    // Check for IO Errors
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot Open File: " + file.errorString());
        return;
    }

    if(!CheckLoadFileValidity(file)) {
        QMessageBox::warning(this, "Warning", "Invalid File Type: Games Do Not Match");
        file.close();
        return;
    }

    BuildSettingsWidget(fileName);
    file.close();
}

void MainWindow::on_locationButton_clicked()
{
    auto pathSplit = currentFile.split('/');
    QString path;

    for (int i = 0; i < pathSplit.length() - 1; i++) {
        path += pathSplit[i] + "/";
    }

    QDesktopServices::openUrl( QUrl::fromLocalFile( path ) );
}

void MainWindow::on_closeButton_clicked()
{
    if (ui->settingsGroupBox->isVisible()) ui->settingsGroupBox->setVisible(false);
}
