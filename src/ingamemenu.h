#ifndef INGAME_MENU_H
#define INGAME_MENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "menubutton.h"

class InGameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit InGameMenu(QWidget* parent = nullptr);
    ~InGameMenu();

signals:
    void resumeGame();
    void quitToMainMenu();
    void restartGame();

private:
    QVBoxLayout* layout;
    MenuButton* resumeButton;
    MenuButton* quitToMainButton;
    MenuButton* restartButton;

    void setupUI();
    void connectSignalsSlots();
};

#endif // INGAME_MENU_H

