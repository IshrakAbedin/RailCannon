#include "GameScene.h"

GameScene::GameScene()
	:m_Status(GameStatus::ONGOING)
{
	m_BG = new Background();
	m_B1 = new BlockingArea("BA1");
	m_B2 = new BlockingArea("BA2");
	m_B3 = new BlockingArea("BA3");

	m_B1->Transform.Translation.y = -9.0f;
	m_B1->Transform.Scale.x = 32.0f;
	m_B1->Transform.Scale.y = 2.0f;

	m_B2->Transform.Translation.x = 16.0f;
	m_B2->Transform.Scale.x = 2.0f;
	m_B2->Transform.Scale.y = 36.0f;

	m_B3->Transform.Translation.x = -16.0f;
	m_B3->Transform.Scale.x = 2.0f;
	m_B3->Transform.Scale.y = 36.0f;

	m_WindGen = m_WindGen = new Wind(0.2f);

	m_TankA = new Tank(1.0f, false, "Player 1", "res/textures/cannon/", m_WindGen);
	m_TankB = new Tank(1.0f, true, "Player 2", "res/textures/cannon/", m_WindGen);


	m_TankA->Transform.Scale = glm::vec3(2.0f);
	m_TankB->Transform.Scale = glm::vec3(2.0f);

	m_TankA->Transform.Translation.x = -12.0f;
	m_TankA->Transform.Translation.y = -3.4f;

	m_TankB->Transform.Translation.x = 12.0f;
	m_TankB->Transform.Translation.y = -3.4f;

	m_TankA->SetLeftBoundary(-13.76f);
	m_TankA->SetRightBoundary(-5.98f);

	m_TankB->SetLeftBoundary(5.98f);
	m_TankB->SetRightBoundary(13.76f);

	m_TankA->RegisterCannonBallCollidable(m_TankB);
	m_TankA->RegisterCannonBallCollidable(m_B1);
	m_TankA->RegisterCannonBallCollidable(m_B2);
	m_TankA->RegisterCannonBallCollidable(m_B3);

	m_TankB->RegisterCannonBallCollidable(m_TankA);
	m_TankB->RegisterCannonBallCollidable(m_B1);
	m_TankB->RegisterCannonBallCollidable(m_B2);
	m_TankB->RegisterCannonBallCollidable(m_B3);

	m_TankA->OnDepossessCallback = [&]() {SetTankActive(m_TankB); };
	m_TankB->OnDepossessCallback = [&]() {SetTankActive(m_TankA); };

	// Game start conditions
	m_TankA->IsControllerOn = true;
	m_TankB->IsControllerOn = false;

	SetTankActive(m_TankA);
	//TankA->OnPossess();
}

GameScene::~GameScene()
{
	delete m_BG;
	delete m_B1;
	delete m_B2;
	delete m_B3;
	delete m_WindGen;
	delete m_TankA;
	delete m_TankB;
}

void GameScene::OnUpdate(float timeStep)
{
	m_BG->OnUpdate(timeStep);
	m_WindGen->OnUpdate(timeStep);
	m_B1->OnUpdate(timeStep);
	m_B2->OnUpdate(timeStep);
	m_B3->OnUpdate(timeStep);
	m_TankA->OnUpdate(timeStep);
	m_TankB->OnUpdate(timeStep);

	UpdateStatus();
}

void GameScene::OnRender()
{
	m_BG->OnRender();
	m_WindGen->OnRender();
	m_B1->OnRender();
	m_B2->OnRender();
	m_B3->OnRender();
	m_TankA->OnRender();
	m_TankB->OnRender();
}

void GameScene::OnImGuiRender()
{
	m_WindGen->OnImGuiRender();
	m_TankA->OnImGuiRender();
	m_TankB->OnImGuiRender();
}

void GameScene::InitializeStats()
{
	m_Status = GameStatus::ONGOING;

	m_TankA->SetHp(1.0f);
	m_TankA->SetMuzzleRotationToMin();
	m_TankA->SetVelocity(0.5f);
	m_TankA->SetMediumBallCount(2);
	m_TankA->SetHeavyBallCount(1);
	m_TankA->Transform.Translation.x = -12.0f;
	m_TankA->Transform.Translation.y = -3.4f;

	m_TankB->SetHp(1.0f);
	m_TankB->SetMuzzleRotationToMin();
	m_TankB->SetVelocity(0.5f);
	m_TankB->SetMediumBallCount(2);
	m_TankB->SetHeavyBallCount(1);
	m_TankB->Transform.Translation.x = 12.0f;
	m_TankB->Transform.Translation.y = -3.4f;

	if (m_CurrentTank == m_TankB) 
	{
		m_TankB->ResetCannonBall();
		m_TankB->OnDepossess();
	}
	else if (m_CurrentTank == m_TankA)
	{
		m_TankA->ResetCannonBall();
		SetTankActive(m_TankA);
	}
	else if(m_CurrentTank == nullptr)
	{
		m_TankA->IsControllerOn = true;
		m_TankB->IsControllerOn = false;

		SetTankActive(m_TankA);
	}
}

void GameScene::SetTankActive(Tank * tank)
{
	m_WindGen->ShuffleWindVelocity();
	m_CurrentTank = tank;
	tank->OnPossess();
}

void GameScene::KeyCallbackRedirect(int key, int scancode, int action, int mods)
{
	if (m_CurrentTank)
		m_CurrentTank->KeyCallbackRedirect(key, scancode, action, mods);
}

void GameScene::UpdateStatus()
{
	if (m_TankA->GetHp() <= 0.001f) // Floating point comparsion problems
	{
		m_Status = GameStatus::PLAYER_TWO_HAS_WON;
		m_CurrentTank = nullptr;
	}
	else if (m_TankB->GetHp() <= 0.001f) // Floating point comparsion problems
	{
		m_Status = GameStatus::PLAYER_ONE_HAS_WON;
		m_CurrentTank = nullptr;
	}
}
