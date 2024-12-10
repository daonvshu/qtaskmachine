#include <qapplication.h>
#include <qfont.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "app.h"
#include "myapplication.h"

#include <qlogcollector.h>
#include <qloggingcategory.h>
#include <dao.h>
#include <qmessagebox.h>

int main(int argc, char* argv[]) {
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
	MyApplication a(argc, argv);

#ifdef Q_OS_WIN
    //设置系统默认字体
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(ncm);
    HRESULT hr;
    hr = SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
    if (hr != 0) {
        auto font = QApplication::font();
#ifdef QT_FEATURE_cxx17
        font.setFamily(QString::fromStdWString(ncm.lfMenuFont.lfFaceName));
#else
        font.setFamily(QString::fromStdString(ncm.lfMenuFont.lfFaceName));
#endif
        QApplication::setFont(font);
    }
#endif

    logcollector::styleConfig
            .windowApp()
            .ide_clion(false)
            .wordWrap(180)
            .projectSourceCodeRootPath(ROOT_PROJECT_PATH)
            ;
    logcollector::QLogCollector::instance().registerLog();

    try {
        dao::_config<dao::ConfigSqliteBuilder>()
                .version(1)
                .databaseName("task_machine_flow_editor")
                .initializeDatabase();
    } catch (dao::DaoException& e) {
        Q_UNUSED(e)
        QMessageBox::critical(nullptr, "warning", "setup database fail:" + e.reason);
        return 0;
    }

    QLoggingCategory::setFilterRules(QStringLiteral("qeventbus.*.info=false"));

    App app;
    app.show();

	return a.exec();
}