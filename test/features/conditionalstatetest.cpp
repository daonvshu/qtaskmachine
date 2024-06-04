#include "conditionalstatetest.h"

#include "conditionalstaterun1.h"
#include "conditionalstaterun2.h"
#include "conditionalstaterun3.h"
#include "conditionalstaterun4.h"
#include "conditionalstaterun5.h"

int ConditionalStateTest::id = qRegisterMetaType<ConditionalStateTest*>();
ConditionalStateTest::ConditionalStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString ConditionalStateTest::getDescription() {
    return u8"<p>所有基础状态具有选择性切换，默认情况下调用target或右移运算符&gt;&gt;只会添加一个目标，选择性切换时调用next或&lt;&lt;运算符：\n</p>"
           "<p><span style='font-weight:bold'>next/operator&lt;&lt;：</span>添加一个切换目标并返回自身，再次调用右移运算符作为目标状态的下一个切换状态。\n</p>"
           "<p><span style='font-weight:bold'>setCondition：</span>选择目标状态，在状态enter时生效，因此需要在enter触发之前调用。\n</p>"
           "<p><span style='font-weight:bold'>setConditionDeferrable：</span>延迟条件切换目标，等同于目标切换在exit时生效，需要在设置切换和条件前调用。\n</p>";
}

QString ConditionalStateTest::getCodeFile() {
    return FEATURES_ROOT_DIR + QString("/conditionalstaterun%1.h").arg(getTypeIndex() + 1);
}

void ConditionalStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    switch (getTypeIndex()) {
        case 0:
            stateMachine = ConditionalStateRun1::run(ui.btn_positive->isChecked());
            break;
        case 1:
            stateMachine = ConditionalStateRun2::run(ui.btn_positive->isChecked());
            break;
        case 2:
            stateMachine = ConditionalStateRun3::run(ui.index_selector->currentIndex());
            break;
        case 3:
            stateMachine = ConditionalStateRun4::run(ui.btn_positive->isChecked());
            break;
        case 4:
            stateMachine = ConditionalStateRun5::run(ui.btn_positive->isChecked());
            break;
    }
}

QWidget *ConditionalStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.buttonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, [&] (QAbstractButton* btn) {
        if (btn == ui.btn_type1 || btn == ui.btn_type2 || btn == ui.btn_type4 || btn == ui.btn_type5) {
            ui.bool_selector_box->setEnabled(true);
            ui.index_selector_box->setEnabled(false);
        } else {
            ui.bool_selector_box->setEnabled(false);
            ui.index_selector_box->setEnabled(true);
        }
        requestCodeReload();
    });
    return widget;
}

int ConditionalStateTest::getTypeIndex() const {
    int index = -1;
    if (ui.btn_type1->isChecked()) {
        index = 0;
    } else if (ui.btn_type2->isChecked()) {
        index = 1;
    } else if (ui.btn_type3->isChecked()) {
        index = 2;
    } else if (ui.btn_type4->isChecked()) {
        index = 3;
    } else if (ui.btn_type5->isChecked()) {
        index = 4;
    }
    return index;
}
