#include "customparsers.h"

QMap<QString, QString> CustomParsers::Parse(QString filePath) {
    // Determine the type of file we're working with
    QString fileType = QFileInfo(filePath).suffix();

    if (fileType == "ini") {
        QSettings iniFile(filePath, QSettings::IniFormat);
        return ParseIni(iniFile);
    }
    else if (fileType == "json") {
        QFile jsonFile(filePath);

        if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw "Invalid Json File";

        QString val = jsonFile.readAll();
        jsonFile.close();
        auto doc = QJsonDocument::fromJson(val.toUtf8());

        return ParseJson(doc);
    }

    throw "Unsupported File Type";
}

void CustomParsers::Save(QString filePath, QMap<QString, QLineEdit*> currentParams) {
    // Determine the type of file we're working with
    QString fileType = QFileInfo(filePath).suffix();

    if (fileType == "ini") {
        QSettings iniFile(filePath, QSettings::IniFormat);
        SaveIni(iniFile, currentParams);
    }
    else if (fileType == "json") {
        QFile jsonFile(filePath);

        if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw "Invalid Json File";

        QString val = jsonFile.readAll();
        jsonFile.close();
        auto doc = QJsonDocument::fromJson(val.toUtf8());

        SaveJson(jsonFile, doc, currentParams);
    }
    else
        throw "Unsupported File Type";
}

QMap<QString, QString> CustomParsers::ParseJson(QJsonDocument &doc) {
    QMap<QString, QString> answer;

    // Get jsonObject
    auto jsonObject = doc.object();

    // Parse it and return the answer
    return JsonParserHelper(jsonObject);
}

QMap<QString, QString> CustomParsers::JsonParserHelper(QJsonObject jsonObject) {
    QMap<QString, QString> answer;

    // Convert jsonObject to a variantMap
    auto jsonMap = jsonObject.toVariantMap();

    // Convert string-variant to string-string map
    for(auto key : jsonMap.keys()) {
        QJsonValueRef val = jsonObject.find(key).value();

        // TODO: DEBUG!!! REMOVE THIS
        // if (answer.count() > 15) return answer;

        if (val.isObject()) {
            auto obj = val.toObject();
            auto map = JsonParserHelper(obj);

            for (auto mapVal : map)
                answer.insert(mapVal, map[mapVal]);

            return map;
        }

        answer.insert(key, jsonMap[key].toString());
    }

    return answer;
}

QMap<QString, QString> CustomParsers::ParseIni(QSettings &file) {
    QMap<QString, QString> answer;
    QVariant val;

    for (auto key : file.allKeys()) {
        try {
            val = file.value(key);
            auto keySplit = key.split("/");
            if (keySplit.length() > 1)
                key = keySplit[1];
            answer.insert(key, val.toString());
        }  catch (...) {
            qDebug() << "Error on key " + key + " (" + val.toString() + ")";
        }

        // TODO: DEBUG!!! REMOVE THIS
        // if (answer.count() > 15) return answer;
    }

    return answer;
}

void CustomParsers::SaveJson(QFile &file, QJsonDocument& doc, QMap<QString, QLineEdit*> currentParams) {
    // Get json object
    auto jsonObject = doc.object();

    // Update values with the currentParams list
    for (auto key : jsonObject.keys()) {
        auto valueRef = jsonObject.find(key).value();
        valueRef = currentParams[key]->text();
    }

    // Update object
    doc.setObject(jsonObject);

    // Write object to file
    if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) throw "Invalid Json File";
    file.write(doc.toJson());
    file.close();
}

void CustomParsers::SaveIni(QSettings &file, QMap<QString, QLineEdit*> currentParams) {
    // Get all groups available in the ini file
    auto allGroups = file.childGroups();

    // Go through all the groups
    for (auto group : allGroups) {
        file.beginGroup(group);

        // Get all the keys withing the group
        auto groupKeys = file.allKeys();

        // Save all keys
        for (auto key : groupKeys)
            file.setValue(key, currentParams[key]->text());

        file.endGroup();
    }
}
