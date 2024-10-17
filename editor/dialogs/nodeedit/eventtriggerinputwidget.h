#pragma once

#include <qobject.h>

#include "ui_eventtriggerinput.h"

#include "data/eventstatepropdata.h"

class EventTriggerInputWidget : public QWidget {
    Q_OBJECT

public:
    explicit EventTriggerInputWidget(const EventTriggerFunction& triggerFunction, QWidget *parent = nullptr);

    void hideIdInput();
    void hideRemoveButton();

    QSpinBox* getIdInput() const;
    QLineEdit* getTriggerInput() const;
    QLineEdit* getCheckInput() const;

    const EventTriggerFunction& getData() const {
        return triggerFunction;
    }

signals:
    void removeRequest();

private slots:
    void on_input_id_valueChanged(int value);
    void on_input_trigger_editingFinished();
    void on_input_check_editingFinished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::EventTriggerInput ui;
    EventTriggerFunction triggerFunction;
};
