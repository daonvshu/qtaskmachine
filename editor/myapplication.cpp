#include "myapplication.h"

#include <qlineedit.h>
#include <qevent.h>
#include <qdebug.h>

bool MyApplication::notify(QObject *object, QEvent *event) {
    if (object->metaObject()->className() == QLatin1String(QLineEdit::staticMetaObject.className())) {
        if (event->type() == QEvent::KeyPress) {
            auto keyEvent = dynamic_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return) {
                dynamic_cast<QLineEdit*>(object)->clearFocus();
            }
        }
    }
    return QApplication::notify(object, event);
}
