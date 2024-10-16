#include "conditionstatepropeditdlg.h"

#include <qevent.h>
#include <qvalidator.h>

ConditionStatePropEditDlg::ConditionStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("条件分支状态属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    branchLayout = new FlowLayout(exUi.branch_list_widget);
    branchLayout->setContentsMargins(0, 0, 0, 0);
    branchLayout->setSpacing(8);

    registerMessageHint(exUi.input_check_func, tr("设置条件函数，返回分支id切换到指定分支路径\n"
                                                  "函数签名：\nQ_INVOKABLE int function()"));
}

void ConditionStatePropEditDlg::setExData(const ConditionStatePropertyData &data) {
    exData = data;
    exUi.input_check_func->setText(exData.conditionFunc());

    auto addBtn = new QPushButton(exUi.branch_list_widget);
    addBtn->setFixedSize(32, 32);
    addBtn->setIcon(QIcon(":/res/add.svg"));
    addBtn->setStyleSheet("QPushButton{background: transparent;} QPushButton:hover{background: #4C4E56;}");
    addBtn->setDefault(false);
    addBtn->setAutoDefault(false);
    connect(addBtn, &QPushButton::clicked, this, [&] {
        addBranchEditItem(-1);
    });
    branchLayout->addWidget(addBtn);

    for (int branch : exData.branchIds()) {
        addBranchEditItem(branch);
    }
}

ConditionStatePropertyData ConditionStatePropEditDlg::getExEditData() const {
    return exData;
}

void ConditionStatePropEditDlg::on_btn_confirm_clicked() {
    exData.conditionFunc = exUi.input_check_func->text();
    exData.branchIds().clear();
    auto lineEdits = exUi.branch_list_widget->findChildren<QLineEdit*>();
    for (auto item : lineEdits) {
        auto branchId = item->text().toInt();
        exData.branchIds().append(branchId);
    }
    BasePropertyEditDlg::on_btn_confirm_clicked();
}

void ConditionStatePropEditDlg::addBranchEditItem(int branchId) {

    auto itemWidget = new QWidget(exUi.branch_list_widget);
    itemWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    itemWidget->setStyleSheet("QWidget{border-radius:4px;} QWidget:hover{background:#4C4E56;}");
    auto itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(6, 4, 6, 6);
    itemLayout->setSpacing(0);

    auto branchEditor = new QLineEdit(QString::number(branchId), itemWidget);
    branchEditor->setStyleSheet("border:none");
    branchEditor->installEventFilter(this);
    auto editorResize = [=] (const QString& text) {
        auto fontMetrics = branchEditor->fontMetrics();
        branchEditor->setFixedWidth(fontMetrics.horizontalAdvance(text) + 16 + 8);
    };
    connect(branchEditor, &QLineEdit::textChanged, this, editorResize);
    editorResize(branchEditor->text());
    auto intValidator = new QIntValidator(branchEditor);
    intValidator->setRange(-1, 1000000);
    branchEditor->setValidator(intValidator);
    itemLayout->addWidget(branchEditor);

    auto removeBtn = new QPushButton(itemWidget);
    removeBtn->setIcon(QIcon(":/res/close.svg"));
    removeBtn->setIconSize(QSize(12, 12));
    removeBtn->setDefault(false);
    removeBtn->setAutoDefault(false);
    connect(removeBtn, &QPushButton::clicked, this, [=](){
        branchLayout->removeWidget(itemWidget);
        itemWidget->deleteLater();
    });

    itemLayout->addWidget(removeBtn);
    branchLayout->addWidget(itemWidget);
}
