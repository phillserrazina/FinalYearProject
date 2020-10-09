#ifndef CUSTOMPARSERS_H
#define CUSTOMPARSERS_H

#include <QMap>
#include <QFile>
#include <QSettings>
#include <QLineEdit>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

class CustomParsers
{
public:
    // CONSTRUCTORS
    CustomParsers() = default;
    ~CustomParsers() = default;

    static QMap<QString, QString> Parse(QString);
    static void Save(QString, QMap<QString, QLineEdit*>);

private:
    // FUNCTIONS
    static QMap<QString, QString> ParseJson(QJsonDocument&);
    static QMap<QString, QString> ParseIni(QSettings&);

    static void SaveJson(QFile&, QJsonDocument&, QMap<QString, QLineEdit*>);
    static void SaveIni(QSettings&, QMap<QString, QLineEdit*>);

    static QMap<QString, QString> JsonParserHelper(QJsonObject);
};

#endif // CUSTOMPARSERS_H
