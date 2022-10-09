#include "Pch.h"

#include "Player.h"
#include "Enemy.h"
#include "Character.h"
#include "Shield.h"
#include "Camera.h"
#include "ModelManager.h"
#include "KeyManager.h"
#include "HitChecker.h"
#include "DeltaTime.h"


// ラインを描く範囲
#define LINE_AREA_SIZE			10000.0f
// ラインの数
#define LINE_NUM			50


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(IS_WINDOW_MODE);
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	ModelManager::GetInstance();
	Character** character = new Character * [1]();
	character[0] = new Player;
	character[0]->Initialize();
	character[1] = new Enemy;
	character[1]->Initialize();
	Camera* camera = new Camera;
	camera->Initialize();
	camera->SetPosition(character);
	HitChecker* hitchecker = new HitChecker(1);
	
	//Player* player = new Player;
	//player->Initialize();
	//Enemy* enemy = new Enemy;
	//enemy->Initialize();
	/*Shield* shield = new Shield;
	shield->Initialize();
	shield->Activate(player->GetPosition(), player->GetDirection());*/

	

	int nowCount, prevCount;
	nowCount = prevCount = GetNowCount();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//deltaTimeの計測
		float deltaTime;
		nowCount = GetNowCount();
		deltaTime = (nowCount - prevCount) / 1000.0f;
		DeltaTime::GetInstace().SetDeltaTime(deltaTime);
		KeyManager::GetInstance().Update();		//入力処理

		//更新処理
		camera->Update(character);
		for (int i = 0; i <= 1; ++i)
		{
			character[i]->Update();
		}

		/*player->Update();
		enemy->Update();*/
		/*shield->Activate(player->GetPosition(), player->GetDirection());
		if (player->IsDefense())
		{
			shield->Update();
		}*/

		//当たり判定
		hitchecker->Check(character, character[0]->GetShieldPointer());

		//画面更新処理
		ClearDrawScreen();
		
		//描画
		for (int i = 0; i <= 1; ++i)
		{
			character[i]->Draw();
		}
		/*player->Draw();
		enemy->Draw();*/
		/*if (player->IsDefense())
		{
			shield->Draw();
		}*/

		//UIデバッグ
		DrawFormatString(50, 70, GetColor(255, 255, 255), "P::position.x : %f", character[0]->GetPosition().x);
		DrawFormatString(50, 90, GetColor(255, 255, 255), "E::position.x : %f", character[1]->GetPosition().x);
		DrawFormatString(50, 110, GetColor(255, 255, 255), "Velocity.x : %f", character[0]->GetVelocity().x);
		DrawFormatString(50, 130, GetColor(255, 255, 255), "Velocity.y : %f", character[0]->GetVelocity().y);
		DrawFormatString(50, 150, GetColor(255, 255, 255), "Velocity.z : %f", character[0]->GetVelocity().z);
		DrawFormatString(50, 170, GetColor(255, 255, 255), "ST : %f", character[0]->GetTrunkPoint());
		DrawFormatString(50, 190, GetColor(255, 255, 255), "HP : %f", character[0]->GetHitPoint());

		//カメラの現在地を表示
		DrawFormatString(300, 70, GetColor(255, 255, 255), "Camera.X : %f\nCamera.Y : %f\nCamera.Z : %f", camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

		// 位置関係が分かるように地面にラインを描画する
		{
			int i;
			VECTOR Pos1;
			VECTOR Pos2;

			SetUseZBufferFlag(TRUE);

			Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, LINE_AREA_SIZE / 2.0f);
			for (i = 0; i <= LINE_NUM; i++)
			{
				DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
				Pos1.x += LINE_AREA_SIZE / LINE_NUM;
				Pos2.x += LINE_AREA_SIZE / LINE_NUM;
			}

			Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			Pos2 = VGet(LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			for (i = 0; i < LINE_NUM; i++)
			{
				DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
				Pos1.z += LINE_AREA_SIZE / LINE_NUM;
				Pos2.z += LINE_AREA_SIZE / LINE_NUM;
			}

			SetUseZBufferFlag(FALSE);
		}

		ScreenFlip();
		prevCount = nowCount;
	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了
	return 0;
}