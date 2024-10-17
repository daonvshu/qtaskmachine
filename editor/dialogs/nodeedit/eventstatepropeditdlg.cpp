#include "eventstatepropeditdlg.h"
#include "eventtriggerinputwidget.h"

EventStatePropEditDlg::EventStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("事件触发状态属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.input_timeout, tr("未设置时一直等待信号触发"));
    registerMessageHint(exUi.input_retry, tr("失败时重新进入自身并触发重试信号"));
    registerMessageHint(exUi.input_retry_callback, tr("重试触发时调用的槽函数"));
}

void EventStatePropEditDlg::setExData(const EventStatePropertyData &data) {
    exData = data;
    exUi.input_timeout->setValue(data.timeout());
    exUi.input_retry->setValue(data.timeoutRetryCount());
    exUi.input_retry_callback->setText(data.timeoutRetryCallback());

    normalTriggerEvent = new EventTriggerInputWidget(exData.normalEvent(), exUi.event_prop_edit_layout->parentWidget());
    normalTriggerEvent->hideIdInput();
    normalTriggerEvent->hideRemoveButton();
    exUi.event_prop_edit_layout->addWidget(normalTriggerEvent, 3, 1);

    errorTriggerEvent = new EventTriggerInputWidget(exData.errorEvent(), exUi.event_prop_edit_layout->parentWidget());
    errorTriggerEvent->hideIdInput();
    errorTriggerEvent->hideRemoveButton();
    exUi.event_prop_edit_layout->addWidget(errorTriggerEvent, 4, 1);

    auto registerTriggerInput = [this] (EventTriggerInputWidget* inputWidget) {
        registerMessageHint(inputWidget->getTriggerInput(), tr("触发该状态退出的信号函数"));
        registerMessageHint(inputWidget->getCheckInput(), tr("设置信号参数检查函数，返回true时表示信号有效\n"
                                                             "函数签名：\n"
                                                             "Q_INVOKABLE bool function(const QVariantList& data)"));
    };
    registerTriggerInput(normalTriggerEvent);
    registerTriggerInput(errorTriggerEvent);
}

EventStatePropertyData EventStatePropEditDlg::getExEditData() const {
    return exData;
}

void EventStatePropEditDlg::on_btn_confirm_clicked() {
    exData.timeout = exUi.input_timeout->value();
    exData.timeoutRetryCount = exUi.input_retry->value();
    exData.timeoutRetryCallback = exUi.input_retry_callback->text();
    exData.normalEvent = normalTriggerEvent->getData();
    exData.errorEvent = errorTriggerEvent->getData();
    BasePropertyEditDlg::on_btn_confirm_clicked();
}
