#pragma once

#include <qobject.h>
#include <qvariant.h>

#include <utility>

template<typename T>
class SettingOperator {
public:
    explicit SettingOperator(QString key, T defaultValue = T());

    T& operator()() {
        return value;
    }

    SettingOperator<T>& operator=(const T& v);

    void save();

    void load();

private:
    QString key;
    T value;
    T defaultValue;
};

class AppSettings {
public:
    static SettingOperator<QStringList> recentFiles;
    static SettingOperator<QString> lastOpenFilePath;
    static SettingOperator<QString> lastExportLogPath;
    static SettingOperator<QString> lastConnectTarget;
    static SettingOperator<int> lastConnectPort;
    static SettingOperator<bool> snapEnabled;
    static SettingOperator<bool> animDirEnabled;

    static void init();

private:
    static void saveValue(const QString& key, const QVariant& value);
    static QVariant loadValue(const QString& key, const QVariant& defaultValue);

    template<typename>
    friend class SettingOperator;
};

template<typename T>
SettingOperator<T>::SettingOperator(QString key, T defaultValue)
    : key(std::move(key)), value(defaultValue), defaultValue(defaultValue)
{}

template<typename T>
SettingOperator<T>& SettingOperator<T>::operator=(const T &v)  {
    value = v;
    AppSettings::saveValue(key, v);
    return *this;
}

template<typename T>
void SettingOperator<T>::save() {
    AppSettings::saveValue(key, value);
}

template<typename T>
void SettingOperator<T>::load() {
    value = AppSettings::loadValue(key, defaultValue).template value<T>();
}