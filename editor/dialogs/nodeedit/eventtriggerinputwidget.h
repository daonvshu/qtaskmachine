#pragma once

#include <qobject.h>

#include "ui_eventtriggerinput.h"

#include "data/eventstatepropdata.h"

class EventTriggerInputWidget : public QWidget {
    Q_OBJECT

public:
    explicit EventTriggerInputWidget(EventTriggerFunction* triggerFunction, QWidget *parent = nullptr);

    void hideRemoveButton();

    QLineEdit* getTriggerInput() const;
    QLineEdit* getCheckInput() const;

signals:
    void removeRequest();

private slots:
    void on_input_trigger_editingFinished();
    void on_input_check_editingFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::EventTriggerInput ui;
    EventTriggerFunction* triggerFunction;
};
