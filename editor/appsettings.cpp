#include "appsettings.h"

#include <qsettings.h>
#include <qstandardpaths.h>

SettingOperator<QStringList> AppSettings::recentFiles("recent_files");
SettingOperator<QString> AppSettings::lastOpenFilePath("last_open_file_path");

static QSettings& getSetting() {
    static QSettings setting(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/settings.ini", QSettings::IniFormat);
    return setting;
}

void AppSettings::saveValue(const QString &key, const QVariant &value) {
    auto& setting = getSetting();
    setting.setValue(key, value);
}

QVariant AppSettings::loadValue(const QString &key, const QVariant &defaultValue) {
    return getSetting().value(key, defaultValue);
}
