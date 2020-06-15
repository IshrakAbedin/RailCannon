#include "GameScene.h"

GameScene::GameScene()
{
	BG = new Background();
	B1 = new BlockingArea("BA1");
	B2 = new BlockingArea("BA2");
	B3 = new BlockingArea("BA3");

	B1->Transform.Translation.y = -9.0f;
	B1->Transform.Scale.x = 32.0f;
	B1->Transform.Scale.y = 2.0f;

	B2->Transform.Translation.x = 16.0f;
	B2->Transform.Scale.x = 2.0f;
	B2->Transform.Scale.y = 36.0f;

	B3->Transform.Translation.x = -16.0f;
	B3->Transform.Scale.x = 2.0f;
	B3->Transform.Scale.y = 36.0f;

	WindGen = WindGen = new Wind(0.2f);

	TankA = new Tank(1.0f, false, "Player 1", "res/textures/cannon/", WindGen);
	TankB = new Tank(1.0f, true, "Player 2", "res/textures/cannon/", WindGen);


	TankA->Transform.Scale = glm::vec3(2.0f);
	TankB->Transform.Scale = glm::vec3(2.0f);

	TankA->Transform.Translation.x = -12.0f;
	TankA->Transform.Translation.y = -3.4f;

	TankB->Transform.Translation.x = 12.0f;
	TankB->Transform.Translation.y = -3.4f;

	TankA->SetLeftBoundary(-13.76f);
	TankA->SetRightBoundary(-5.98f);

	TankB->SetLeftBoundary(5.98f);
	TankB->SetRightBoundary(13.76f);

	TankA->RegisterCannonBallCollidable(TankB);
	TankA->RegisterCannonBallCollidable(B1);
	TankA->RegisterCannonBallCollidable(B2);
	TankA->RegisterCannonBallCollidable(B3);

	TankB->RegisterCannonBallCollidable(TankA);
	TankB->RegisterCannonBallCollidable(B1);
	TankB->RegisterCannonBallCollidable(B2);
	TankB->RegisterCannonBallCollidable(B3);

	TankA->OnDepossessCallback = [&]() {SetTankActive(TankB); };
	TankB->OnDepossessCallback = [&]() {SetTankActive(TankA); };

	// Game start conditions
	TankA->IsControllerOn = true;
	TankB->IsControllerOn = false;

	SetTankActive(TankA);
	//TankA->OnPossess();
}

GameScene::~GameScene()
{
	delete BG;
	delete B1;
	delete B2;
	delete B3;
	delete WindGen;
	delete TankA;
	delete TankB;
}

void GameScene::OnUpdate(float timeStep)
{
	BG->OnUpdate(timeStep);
	WindGen->OnUpdate(timeStep);
	B1->OnUpdate(timeStep);
	B2->OnUpdate(timeStep);
	B3->OnUpdate(timeStep);
	TankA->OnUpdate(timeStep);
	TankB->OnUpdate(timeStep);
}

void GameScene::OnRender()
{
	BG->OnRender();
	WindGen->OnRender();
	B1->OnRender();
	B2->OnRender();
	B3->OnRender();
	TankA->OnRender();
	TankB->OnRender();
}

void GameScene::OnImGuiRender()
{
	WindGen->OnImGuiRender();
	TankA->OnImGuiRender();
	TankB->OnImGuiRender();
}

void GameScene::InitializeStats()
{
	TankA->SetHp(1.0f);
	TankA->SetMuzzleRotationToMin();
	TankA->SetVelocity(0.5f);
	TankA->SetMediumBallCount(2);
	TankA->SetHeavyBallCount(1);
	TankA->Transform.Translation.x = -12.0f;
	TankA->Transform.Translation.y = -3.4f;

	TankB->SetHp(1.0f);
	TankB->SetMuzzleRotationToMin();
	TankB->SetVelocity(0.5f);
	TankB->SetMediumBallCount(2);
	TankB->SetHeavyBallCount(1);
	TankB->Transform.Translation.x = 12.0f;
	TankB->Transform.Translation.y = -3.4f;

	if (CurrentTank == TankB) 
	{
		TankB->ResetCannonBall();
		TankB->OnDepossess();
	}
	else if (CurrentTank == TankA)
	{
		TankA->ResetCannonBall();
		SetTankActive(TankA);
	}
}

void GameScene::SetTankActive(Tank * tank)
{
	WindGen->ShuffleWindVelocity();
	CurrentTank = tank;
	tank->OnPossess();
}

void GameScene::KeyCallbackRedirect(int key, int scancode, int action, int mods)
{
	if (CurrentTank)
		CurrentTank->KeyCallbackRedirect(key, scancode, action, mods);
}
