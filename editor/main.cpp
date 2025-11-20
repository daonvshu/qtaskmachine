#include <qapplication.h>
#include <qfont.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "app.h"
#include "myapplication.h"
#include "appsettings.h"

#include <qlogcollector/server/logcollector.h>
#include <qloggingcategory.h>
#include <dao.h>
#include <qmessagebox.h>

QLOGCOLLECTOR_USE_NAMESPACE

int main(int argc, char* argv[]) {
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
	MyApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/logo.ico"));

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

    LogCollector::styleConfig
        .wordWrap(180)
        .projectSourceCodeRootPath(ROOT_PROJECT_PATH)
    ;
    LogCollector::addOutputTarget(OutputTarget::currentConsoleOutput(Ide::clion));
    LogCollector::registerLog();

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

    AppSettings::init();

    App app;
    app.show();

	return a.exec();
}