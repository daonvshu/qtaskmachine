#pragma once

#include <qobject.h>
#include <qvariant.h>

#include <utility>

template<typename T>
class SettingOperator {
public:
    explicit SettingOperator(QString key);

    T& operator()() {
        return value;
    }

    SettingOperator<T>& operator=(const T& v);

    void save();

    void load();

private:
    QString key;
    T value;
};

class AppSettings {
public:
    static SettingOperator<QStringList> recentFiles;
    static SettingOperator<QString> lastOpenFilePath;
    static SettingOperator<QString> lastExportLogPath;

    static void init();

private:
    static void saveValue(const QString& key, const QVariant& value);
    static QVariant loadValue(const QString& key, const QVariant& defaultValue);

    template<typename>
    friend class SettingOperator;
};

template<typename T>
SettingOperator<T>::SettingOperator(QString key)
    : key(std::move(key)), value(T())
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
    value = AppSettings::loadValue(key, T()).template value<T>();
}