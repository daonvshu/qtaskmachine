#include "multieventstatepropeditdlg.h"
#include "eventtriggerinputwidget.h"

MultiEventStatePropEditDlg::MultiEventStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("复合事件状态属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    connect(exUi.btn_event_add, &QPushButton::clicked, this, [&] {
        addEventEditItem(EventTriggerFunction());
    });
}

void MultiEventStatePropEditDlg::setExData(const MultiEventStatePropertyData &data) {
    exData = data;
    for (const auto& event : data.events()) {
        addEventEditItem(event);
    }
}

MultiEventStatePropertyData MultiEventStatePropEditDlg::getExEditData() const {
    return exData;
}

void MultiEventStatePropEditDlg::on_btn_confirm_clicked() {
    exData.events().clear();
    for (int i = 0; i < exUi.layout_bind_events->count() - 2; ++i) {
        auto item = exUi.layout_bind_events->itemAt(i);
        auto widget = item->widget();
        auto editor = dynamic_cast<EventTriggerInputWidget*>(widget);
        if (editor) {
            exData.events().append(editor->getData());
        }
    }
    BasePropertyEditDlg::on_btn_confirm_clicked();
}

void MultiEventStatePropEditDlg::addEventEditItem(const EventTriggerFunction &data) {
    auto editor = new EventTriggerInputWidget(data, exUi.layout_bind_events->parentWidget());
    exUi.layout_bind_events->insertWidget(exUi.layout_bind_events->count() - 2, editor);
    connect(editor, &EventTriggerInputWidget::removeRequest, this, [&, editor] {
        exUi.layout_bind_events->removeWidget(editor);
        editor->deleteLater();
    });
}
