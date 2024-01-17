#include "demos.h"

#include "highlighter.h"

#include <qdebug.h>
#include <qfile.h>
#include <qdatetime.h>
#include <qmath.h>

QPointer<Demos> appPtr;

void customMessageHandle(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (appPtr) {
        QString log("[%1] -> %2");
        log = log.arg(QDateTime::currentDateTime().toString("HH:mm:ss.zzz"), msg);
        appPtr->appendLog(log);
    }
}

Demos::Demos(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    auto font = ui.description->font();
    font.setPixelSize(14);
    ui.description->setFont(font);

    appPtr = this;

    qInstallMessageHandler(customMessageHandle);

    new CppHighlighter(ui.code->document());

    treeModel = new QStandardItemModel(this);
    ui.treeView->setModel(treeModel);

    auto baseStateGroup = new QStandardItem(u8"基础状态");
    treeModel->appendRow(baseStateGroup);
    {
        auto item = new QStandardItem(u8"直接状态");
        item->setData("DirectStateTest", Qt::UserRole + 1);
        baseStateGroup->appendRow(item);

        item = new QStandardItem(u8"延时状态");
        item->setData("DelayStateTest", Qt::UserRole + 1);
        baseStateGroup->appendRow(item);

        item = new QStandardItem(u8"事件触发状态");
        item->setData("EventStateTest", Qt::UserRole + 1);
        baseStateGroup->appendRow(item);

        item = new QStandardItem(u8"分组状态");
        item->setData("GroupStateTest", Qt::UserRole + 1);
        baseStateGroup->appendRow(item);
    }

    connect(ui.treeView, &QTreeView::clicked, this, &Demos::loadPage);
    ui.treeView->expandAll();
}

void Demos::loadPage(const QModelIndex &index) {
    auto pageClass = index.data(Qt::UserRole + 1).toByteArray();

    int type = QMetaType::type(pageClass + '*');
    const auto metaObj = QMetaType::metaObjectForType(type);
    if (metaObj == nullptr) {
        return;
    }

    auto obj = metaObj->newInstance(Q_ARG(QObject*, this));
    auto widget = dynamic_cast<PageLoadInterface*>(obj);
    if (widget == nullptr) {
        return;
    }
    widget->requestCodeReload = [&] {
        rendPage();
    };

    delete innerPage;
    innerPage = widget;
    rendPage();
    loadExtraInputUi();

    ui.label_title->setText(index.data().toString());
}

void Demos::rendPage() {
    ui.description->setText(innerPage->getDescription());
    fitHeight(ui.description);

    QFile file(innerPage->getCodeFile());
    file.open(QIODevice::ReadOnly);
    ui.code->setText(file.readAll());
    file.close();

    ui.log->clear();
}

void Demos::loadExtraInputUi() {
    while (auto item = ui.extra_params_layout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
    if (auto widget = innerPage->getExtraInputWidgets()) {
        ui.extra_params_layout->addWidget(widget);
    }
}

void Demos::appendLog(const QString &log) {
    ui.log->append(log);
    fitHeight(ui.log, ui.log->minimumHeight());
}

void Demos::on_btn_run_clicked() {
    ui.log->clear();
    if (innerPage) {
        innerPage->run();
    }
}

void Demos::fitHeight(QTextBrowser *browser, int minHeight) {
    browser->setFixedHeight(qMax(minHeight, qFloor(browser->document()->size().height()) + 12));
}

void Demos::showEvent(QShowEvent *event) {
    auto firstIndex = treeModel->indexFromItem(treeModel->item(0)->child(0));
    ui.treeView->setCurrentIndex(firstIndex);
    loadPage(firstIndex);
}

void Demos::resizeEvent(QResizeEvent *event) {
    fitHeight(ui.description);
    fitHeight(ui.log, ui.log->minimumHeight());
}
