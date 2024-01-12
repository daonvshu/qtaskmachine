#include <QApplication>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "demos.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

#ifdef Q_OS_WIN
    //设置系统默认字体
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(ncm);
    HRESULT hr;
    hr = SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
    if (hr != 0) {
        auto font = QApplication::font();
        font.setFamily(QString::fromLocal8Bit(ncm.lfMenuFont.lfFaceName));
        QApplication::setFont(font);
    }
#endif

    Demos demos;
    demos.show();

	return a.exec();
}