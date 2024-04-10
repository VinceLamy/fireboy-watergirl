#include <QWidget>
#include <QKeyEvent>

#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

#include <QObject>
#include <QEvent>

class KeyboardHandler : public QObject
{
    Q_OBJECT

public:
    explicit KeyboardHandler(QObject* parent = nullptr);
    ~KeyboardHandler() override;

signals:
    void KeyPressed(int key);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif KEYBOARDHANDLER_H