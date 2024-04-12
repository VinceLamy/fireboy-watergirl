 #ifndef MAP_H
#define MAP_H

#include <vector>

#include "character.h"
#include "tile.h"
#include "controller.h"
#include "gate.h"
#include "pool.h"
#include "exit.h"
#include "codelock.h"
#include "codegiver.h"
#include <string>
#include <QWidget>
#include <QtWidgets>

//using namespace std;

class Map : public QGraphicsScene
{
	Q_OBJECT
public:
	Map(const char* nomNiveau, QObject* parent = nullptr);
	Map(int code, const char* nomNiveau, QObject* parent = nullptr);
	~Map();

	void ReadMap();
	void ShowMap();

	std::vector<std::vector<Tile*>>* GetGrid();
	
	void AddTile(int x, int y);
	void AddCharacter(int x, int y, Element e);
	void AddExit(int x, int y);
	void AddPool(int x, int y, Element e);
	void AddWall(int x, int y);
	void AddGate(int x, int y, int size, Orientation o);
	void AddLever(int x, int y);
	void AddButton(int x, int y);
	void AddCodeLock(int x, int y, std::vector<CodeGiver*>);
	void AddCodeGiver(int x, int y);

	CodeLock* GetCodeLock();
	

	Character* GetActiveCharacter();

	/*void StopTimer();*/

signals:
	void GameOver();
	void LevelFinished();
	void SendingDigits(const QString& s);
	void OpenInGameMenu();
	void SendCodeLockToGame(CodeLock* code);

public slots:
	void UpdateScene();
	void SendGameOverToGame();
	void SwitchCharacter();
	void CheckGates();
	void CheckButtons();
	void CheckExits();
	void SendDigitsToGame(const QString& s);
	void GoingToOpenInGameMenu();
	void CatchCodeLock(CodeLock* code);
	
private:
	const char* _fileName;
	bool levelFinished = false;
	int _code;
	std::vector<std::vector<Tile*>> _grid;
	std::vector<Controller*> _lastControllers;
	std::vector<CodeGiver*> _lastCodeGiver;
	std::vector<Button*> _button;
	Character* _waterGirl;
	Character* _fireBoy;
	CodeLock* _codeLock;
	std::vector<Pool*> _pool;
	std::vector<Gate*> _gate;
	std::vector<Exit*> _exit;
	
	QPixmap wallPixmap;
	QPixmap waterPixamp;
	QPixmap lavaPixmap;
	QPixmap gooPixmap;
	QPixmap gatePixmap;
	QPixmap fireJpPixmap;
	QPixmap waterAlexPixmap;
	QPixmap codeGiverPixmap;
	QPixmap exitPixmap;
	QPixmap buttonPixmap;
	QPixmap leverPixmap;
	QPixmap codeLockPixmap;
	QPixmap backgroundPixmap;
	QPixmap emptyPixmap;

};

#endif MAP_H