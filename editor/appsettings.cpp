#include "appsettings.h"

#include <qsettings.h>
#include <qstandardpaths.h>
#include <qapplication.h>
#include <qdir.h>

SettingOperator<QStringList> AppSettings::recentFiles("recent_files");
SettingOperator<QString> AppSettings::lastOpenFilePath("last_open_file_path");
SettingOperator<QString> AppSettings::lastExportLogPath("last_export_log_path");

static QSettings& getSetting() {
    static QSettings setting([] () -> QString {
        auto dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir saveDir(dir);
        if (!saveDir.exists()) {
            saveDir.mkpath(dir);
        }
        return dir + "/settings.ini";
    } (), QSettings::IniFormat);
    return setting;
}

void AppSettings::saveValue(const QString &key, const QVariant &value) {
    auto& setting = getSetting();
    setting.setValue(key, value);
}

QVariant AppSettings::loadValue(const QString &key, const QVariant &defaultValue) {
    return getSetting().value(key, defaultValue);
}

void AppSettings::init() {
    AppSettings::recentFiles.load();
    AppSettings::lastOpenFilePath.load();
    AppSettings::lastExportLogPath.load();
}
