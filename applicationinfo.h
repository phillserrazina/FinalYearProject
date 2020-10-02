#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QString>
#include <QLabel>
#include <QPixmap>

class ApplicationInfo
{
public:
    ApplicationInfo() = default;
    ApplicationInfo(QString, QString, QString);

    QString GetName();
    QPixmap GetIcon();
    QString GetFilePath();
    void SetFilePath(QString);

private:
    QString name;
    QString iconPath;
    QString filePath;
};

#endif // APPLICATIONINFO_H
