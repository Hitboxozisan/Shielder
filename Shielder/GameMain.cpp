#include "Pch.h"
#include "GameMain.h"

GameMain::GameMain(SceneManager* const sceneManager)
	:SceneBase(sceneManager)
	,state(START)
	,pUpdate(nullptr)
{
}

GameMain::~GameMain()
{
	if (character != nullptr)
	{
		Finalize();
	}
}

void GameMain::Initialize()
{

}

void GameMain::Finalize()
{
}

void GameMain::Activate()
{
}

void GameMain::Deactivate()
{
}

void GameMain::Update()
{
}

void GameMain::Draw()
{
}

void GameMain::UpdateStart()
{
}

void GameMain::UpdateGame()
{
}

void GameMain::UpdateGameOver()
{
}

void GameMain::UpdateResult()
{
}
