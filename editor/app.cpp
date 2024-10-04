#include "app.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qdebug.h>
#include <qtimer.h>

App::App(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
}

void App::on_btn_min_clicked() {
    showMinimized();
}

void App::on_btn_max_clicked() {
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void App::on_btn_close_clicked() {
    close();
}

void App::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    QTimer::singleShot(0, this, [=]() {
        ui.btn_max->setIcon(isMaximized() ? QIcon(":/res/maxsize2.svg") : QIcon(":/res/maxsize1.svg"));
    });
}
