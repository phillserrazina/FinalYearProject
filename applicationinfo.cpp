#include "applicationinfo.h"

ApplicationInfo::ApplicationInfo(QString n, QString ip, QString fp)
    : name(n), iconPath(ip), filePath(fp)
{

}

QString ApplicationInfo::GetName() { return name; }

QPixmap ApplicationInfo::GetIcon() {
    QPixmap img(iconPath);
    return img;
}

QString ApplicationInfo::GetFilePath() { return filePath; }

void ApplicationInfo::SetFilePath(QString path) { filePath = path; }
