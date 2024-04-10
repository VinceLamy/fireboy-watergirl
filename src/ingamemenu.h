#ifndef INGAME_MENU_H
#define INGAME_MENU_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class InGameMenu : public QWidget
{
    Q_OBJECT

public:
    explicit InGameMenu(QWidget* parent = nullptr);
    ~InGameMenu();

signals:
    void resumeGame();
    void quitToMainMenu();

private:
    QVBoxLayout* layout;
    QPushButton* resumeButton;
    QPushButton* quitToMainButton;

    void setupUI();
    void connectSignalsSlots();
};

#endif // INGAME_MENU_H

