#pragma once

#include "component/Drawable.h"

#include "component/Tank.h"
#include "component/Background.h"
#include "component/Wind.h"
#include "component/BlockingArea.h"

class GameScene : public Drawable
{
private:
	Tank* CurrentTank;
	Tank* TankA;
	Tank* TankB;
	Wind* WindGen;
	Background* BG;
	BlockingArea* B1;
	BlockingArea* B2;
	BlockingArea* B3;

public:
	GameScene();
	~GameScene();

	void OnUpdate(float timeStep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void InitializeStats();
	void SetTankActive(Tank* tank);
	void KeyCallbackRedirect(int key, int scancode, int action, int mods);
};
