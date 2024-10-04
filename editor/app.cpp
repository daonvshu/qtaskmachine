#include "app.h"

#include <QWKWidgets/widgetwindowagent.h>

App::App(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
}
