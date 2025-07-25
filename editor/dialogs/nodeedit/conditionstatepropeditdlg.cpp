#include "conditionstatepropeditdlg.h"

#include <qevent.h>
#include <qvalidator.h>

ConditionStatePropEditDlg::ConditionStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("条件分支状态属性设置"));
    ui.call_on_non_block->setEnabled(true);

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    connect(exUi.btn_branch_add, &QPushButton::clicked, this, [&] {
        addBranchEditItem(-1, QString());
    });

    registerMessageHint(exUi.input_check_func, tr("设置条件函数，返回分支id切换到指定分支路径\n"
                                                  "如果函数不以‘()’结尾，则当做属性读取其值作为Id，属性类型仅支持int、bool、QString\n"
                                                  "如果属性是QString类型，则以分支名称作为判断依据\n"
                                                  "函数签名：Q_INVOKABLE int function()"));
}

void ConditionStatePropEditDlg::setExData(const ConditionStatePropertyData &data) {
    exData = data;
    exUi.input_check_func->setText(exData.conditionFunc());

    for (int i = 0; i < qMin(exData.branchIds().size(), exData.branchNames().size()); i++) {
        addBranchEditItem(exData.branchIds().at(i), exData.branchNames().at(i));
    }
}

ConditionStatePropertyData ConditionStatePropEditDlg::getExEditData() const {
    return exData;
}

void ConditionStatePropEditDlg::on_btn_confirm_clicked() {
    exData.conditionFunc = exUi.input_check_func->text();
    exData.branchIds().clear();
    exData.branchNames().clear();
    for (int i = 0; i < exUi.layout_branch_list->count() - 1; i++) {
        auto item = exUi.layout_branch_list->itemAt(i)->widget();
        if (auto branchItemWidget = dynamic_cast<ConditionBranchItemWidget*>(item)) {
            exData.branchIds().append(branchItemWidget->getId());
            exData.branchNames().append(branchItemWidget->getName());
        }
    }
    BasePropertyEditDlg::on_btn_confirm_clicked();
}

void ConditionStatePropEditDlg::addBranchEditItem(int branchId, const QString& name) {
    auto branchItemWidget = new ConditionBranchItemWidget(branchId, name, exUi.widget);
    exUi.layout_branch_list->insertWidget(exUi.layout_branch_list->count() - 1, branchItemWidget);
    connect(branchItemWidget, &ConditionBranchItemWidget::removeRequest, this, [&, branchItemWidget] {
        exUi.layout_branch_list->removeWidget(branchItemWidget);
        branchItemWidget->deleteLater();
    });
}

ConditionBranchItemWidget::ConditionBranchItemWidget(int id, const QString& name, QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.input_id->setText(QString::number(id));
    ui.input_name->setText(name);

    connect(ui.btn_remove, &QPushButton::clicked, this, &ConditionBranchItemWidget::removeRequest);
}

int ConditionBranchItemWidget::getId() const {
    return ui.input_id->text().toInt();
}

QString ConditionBranchItemWidget::getName() const {
    return ui.input_name->text();
}
