#include "Pch.h"
#include "GameMain.h"

#include "KeyManager.h"
#include "SceneManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Character.h"
#include "Camera.h"
#include "HitChecker.h"
#include "ModelManager.h"
#include "Bullet.h"
#include "BulletCreater.h"

const int PLAYER_AMOUNT = 1;
const int ENEMY_AMOUNT = 1;
const int CHARACTER_AMOUNT = PLAYER_AMOUNT + ENEMY_AMOUNT;
const float MAX_BULLET_AMOUNT = 3;

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
	//�J�����N���X
	camera = new Camera();
	camera->Initialize();

	//�e�����N���X
	bulletCreater = new BulletCreater(&activeBullet, &deactiveBullet);

	//�L�����N�^�[�N���X
	character = new Character * [CHARACTER_AMOUNT]();

	//�v���C���[
	for (int i = 0; i < PLAYER_AMOUNT; i++)
	{
		character[i] = new Player();
		character[i]->Initialize();
	}

	//�G�l�~�[
	for (int i = PLAYER_AMOUNT; i < CHARACTER_AMOUNT; ++i)
	{
		character[i] = new Enemy();
		character[i]->Initialize();
	}

	//���
	for (int i = 0; i < MAX_BULLET_AMOUNT; ++i)
	{
		deactiveBullet.push_back(new Bullet());
		deactiveBullet.back()->Initialize();
	}

	//�����蔻��N���X
	hitchecker = new HitChecker(CHARACTER_AMOUNT);

}

void GameMain::Finalize()
{
	for (int i = 0; i < CHARACTER_AMOUNT; ++i)
	{
		character[i]->Finalize();
		SafeDelete(character[i]);
	}


}

void GameMain::Activate()
{
	state = START;
	pUpdate = &GameMain::UpdateStart;

#ifdef DEBUG
	state = GAME;
	pUpdate = &GameMain::UpdateGame;
#endif
}

void GameMain::Deactivate()
{
	for (int i = 0; i < CHARACTER_AMOUNT; ++i)
	{
		
	}
}

void GameMain::Update()
{
	//ESC�������ꂽ��^�C�g����
	if (KeyManager::GetInstance().CheckPressed(KEY_INPUT_ESCAPE))
	{
		parent->SetNextScene(SceneManager::TITLE);
	}

	//�����������
	if (pUpdate != nullptr)
	{
		(this->*pUpdate)();	//��Ԃ��Ƃ̍X�V����
	}
}

void GameMain::Draw()
{

}

void GameMain::UpdateStart()
{

}

void GameMain::UpdateGame()
{
	for (int i = 0; i < CHARACTER_AMOUNT; ++i)
	{
		character[i]->Update();
	}

	//�A�N�e�B�u�Ȓe�̍X�V����
	for (auto itr = activeBullet.begin(); itr != activeBullet.end();)
	{
		//�����Ă������A�N�e�B�u���X�g
		if ((*itr)->Update() == false)
		{
			(*itr)->Deactivate();
			deactiveBullet.push_back(*itr);
			itr = activeBullet.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	//�v���C���[������ł����
	

	hitchecker->Check(character, character[0]->GetShieldPointer());
	camera->Update(character);
}

void GameMain::UpdateGameOver()
{
	
}

void GameMain::UpdateResult()
{
}
