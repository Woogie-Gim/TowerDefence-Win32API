#pragma once
#include "Scene.h"
#include "EnemySpawner.h"
#include "Map.h"
#include "BgmPlayer.h"
#include "LifeUI.h"

class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene() override;


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	void OnLButtonDown(int mx, int my);
	void RestartGame();

private:
	void InitWorld();

private:
	vector<pair<POINT, POINT>> _lines;

	bool _setOrigin = true;
	POINT _lastPos = {};

	EnemySpawner _spawner;
	Map _map;

	BgmPlayer  _bgm;

	LifeUI       _lifeUI;
	bool _gameOver = false;
	float        _restartCountdown = 0.0f;
};

