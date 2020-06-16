#pragma once

#include "component/Drawable.h"

#include "component/Tank.h"
#include "component/Background.h"
#include "component/Wind.h"
#include "component/BlockingArea.h"

enum class GameStatus
{
	ONGOING = 0, PLAYER_ONE_HAS_WON = 1, PLAYER_TWO_HAS_WON = 2
};

class GameScene : public Drawable
{
private:
	Tank* m_CurrentTank;
	Tank* m_TankA;
	Tank* m_TankB;
	Wind* m_WindGen;
	Background* m_BG;
	BlockingArea* m_B1;
	BlockingArea* m_B2;
	BlockingArea* m_B3;

	GameStatus m_Status;

public:
	GameScene();
	~GameScene();

	void OnUpdate(float timeStep) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void InitializeStats();
	void SetTankActive(Tank* tank);
	void KeyCallbackRedirect(int key, int scancode, int action, int mods);

	inline Tank* GetTankA() const { return m_TankA; }
	inline Tank* GetTankB() const { return m_TankB; }

	inline GameStatus GetStatus() const { return m_Status; }
	inline void SetStatus(const GameStatus status) { m_Status = status; }

	void UpdateStatus();
};
