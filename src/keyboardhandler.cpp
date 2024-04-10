#include "keyboardhandler.h"

#include "keyboardhandler.h"
#include <QKeyEvent>

KeyboardHandler::KeyboardHandler(QObject* parent) : QObject(parent)
{
}

KeyboardHandler::~KeyboardHandler()
{
}

bool KeyboardHandler::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        emit KeyPressed(keyEvent->key());
        return true; // Event handled
    }
    // If the event is not a key press event, pass it to the base class
    return QObject::eventFilter(obj, event);
}