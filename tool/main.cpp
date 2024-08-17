#include <qapplication.h>
#include <qfont.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "app.h"
#include "myapplication.h"

int main(int argc, char* argv[]) {
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

    App app;
    app.show();

	return a.exec();
}