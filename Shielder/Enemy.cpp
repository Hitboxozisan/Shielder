#include "Pch.h"
#include <chrono>
#include <thread>
#include <random>
#include "Enemy.h"

#include "ModelManager.h"
#include "DeltaTime.h"

const float Enemy::COLLIDE_RADIUS = 50.0f;
const float Enemy::NORMAL_SPEED = 3.0f;
const float Enemy::DEFENSE_SPEED = 2.0f;
const float Enemy::JUMP_DIRECTION_Y = -20.0f;
const float Enemy::STOP_VELOCITY = 0.5f;
const float Enemy::FRICTION_FORCE = 0.05f;
const float Enemy::GRAVITY = 0.1f;
const float Enemy::TRUNK_POINT = 100.0f;
const float Enemy::DECREMENT_TRUNK_POINT = 10.0f;

Enemy::Enemy()
	:assaultCount(0)
	,movedDistance(0.0f)
	,stopTime(0.0f)
	,stopMove()
	,currentRightPosition()
	,physical(FINE)
	,state(NONE)
	,attackType()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	vec = LEFT;
	speed = NORMAL_SPEED;
	trunkPoint = TRUNK_POINT;
	position = VGet(2400.0f, 1.0f, 100.0f);
	nextPosition = position;
	direction = ZERO_VECTOR;
	nextDirction = direction;

	state = NORMAL;
	attackType = JUDGE;

	pUpdate = &Enemy::UpdateAttack;

	modelHandle = MV1DuplicateModel(ModelManager::GetInstance().GetModelHandle(ModelManager::ENEMY));
	//読み込み失敗でエラー
	if (modelHandle < 0)
	{
		printfDx("モデルデータの読み込み_Enemy\n");
	}

	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));

	//当たり判定球を設定
	collisionSphere.localCenter = VGet(0.0f, 0.0f, 0.0f);
	collisionSphere.radius = COLLIDE_RADIUS;
	collisionSphere.worldCenter = position;
}

void Enemy::Update()
{
	if (this->pUpdate != nullptr)
	{
		switch (physical)
		{
		case FINE:
			(this->*pUpdate)();
			break;
		case CAUTION:
			(this->*pUpdate)();
			break;
		case DENGER:
			(this->*pUpdate)();
			break;
		}
	}

	
	/*if (!stopMove)
	{
		Move();
		
		InputAction();
		MoveFinish();
	}*/
	
}

void Enemy::Draw()
{
	MV1DrawModel(modelHandle);

	//UI
	DrawFormatString(300, 130, GetColor(255, 255, 255), "E : %f", this->GetTrunk(), TRUE);		//体幹ゲージ表示
	DrawFormatString(300, 170, GetColor(255, 255, 255), "MD : %f", movedDistance, TRUE);

	//当たり判定デバック描画
	DrawSphere3D(collisionSphere.worldCenter, collisionSphere.radius,
		8, GetColor(0, 255, 0), 0, FALSE);
}

void Enemy::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	stopTime = 0.0f;
	//移動を止める
	stopMove = true;
}

void Enemy::OnHitShield(const VECTOR& adjust)
{

	//前回のvelocityをリセットする
	velocity = ZERO_VECTOR;
	stopTime = 0.0f;
	stopMove = true;

	VECTOR force = adjust;
	force.y = 0.0f;			//変な方向に動かないようにする
	force.z = 0.0f;			//変な方向に動かないようにする

	force = VScale(force, -3.0f);

	//後退させる
	velocity = VAdd(velocity, force);
	

	trunkPoint -= DECREMENT_TRUNK_POINT;		//体幹ゲージ減少
	state = SLIDE;
	pUpdate = &Enemy::UpdateSlide;
	
}

//const bool Enemy::IsCollidableState() const
//{
//	return false;
//}

void Enemy::Move()
{
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		nextDirction = VNorm(inputDirection);
	}
	else
	{
		nextDirction = inputDirection;
	}

	nextPosition = VAdd(position, VScale(nextDirction, speed));
}

void Enemy::MoveFinish()
{
	prevPosition = position;
	position = nextPosition;
	direction = nextDirction;

	//位置を設定
	MV1SetPosition(modelHandle, position);

	//移動キーを押していない場合は向きを固定する
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		MV1SetRotationYUseDir(modelHandle, direction, 0.0f);
	}

}

/// <summary>
/// 突進攻撃
/// </summary>
void Enemy::Assault()
{
	
	VECTOR force = VGet(1.0f, 0.0f, 0.0f);			//移動量を設定
	force.y = 0.0f;			//変な方向に動かないようにする
	force.z = 0.0f;			//変な方向に動かないようにする

	//エネミーの現在地が右端寄りなら
	if (currentRightPosition)
	{
		force = VScale(force, 0.5f);		//左側に力を与える
	}
	else
	{
		force = VScale(force, -0.5f);		//右側に力を与える
	}
	
	//突進させる
	if (movedDistance <= 300.0f && movedDistance >= -300.0f)
	{
		stopTime = 0.0f;
		velocity = VAdd(velocity, force);
		nextPosition = VAdd(nextPosition, velocity);
	}
	
	//停止時間を超えたら突進再開
	if (stopTime >= 1.0f)
	{
		++assaultCount;
		movedDistance = 0.0f;
		force = VGet(1.0f, 0.0f, 0.0f);
	}

	//3回突進したら次の行動に移る
	if (assaultCount >= 3.0f)
	{
		attackType = JUDGE;
	}

	stopTime += DeltaTime::GetInstace().GetDeltaTime();
	movedDistance += velocity.x;			//進んだ距離を加算
}

void Enemy::Back()
{
	//乱数用変数
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<int> next(0, ATTACK_AMOUST);

	int attackAmoust = ATTACK_AMOUST;
	returnForce.y -= GRAVITY;

	//画面端まで移動
	if (position.x >= SCREEN_LEFTMOST && position.x <= SCREEN_RIGHTMOST)
	{
	}
	else
	{
		returnForce.x = 0.0f;		//横移動を停止させる
	}

	nextPosition = VAdd(nextPosition, returnForce);			//画面端まで移動

	//地面に着地したら落下を止める
	if (nextPosition.y <= 0.0f)
	{
		velocity = ZERO_VECTOR;
		assaultCount = 0.0f;		//突進回数をリセット
		returnForce = ZERO_VECTOR;
		position.y = 0.0f;
		nextPosition.y = 0.0f;
		int nextAttack = next(eng);	//次の行動を指定(未実装)
		attackType = ASSAULT;		//次の状態に移行する
	}
}

void Enemy::Slide()
{
	//滑らせる
	if (velocity.x <= 0.0f)
	{
		velocity.x += FRICTION_FORCE;
	}
	else
	{
		velocity.x -= FRICTION_FORCE;
	}

	//止まったら画面端状態に戻る
	if (VSquareSize(velocity) <= STOP_VELOCITY)
	{
		velocity = ZERO_VECTOR;
		state = NORMAL;
		attackType = JUDGE;
		pUpdate = &Enemy::UpdateAttack;
	}

	nextPosition = VAdd(nextPosition, velocity);
}

void Enemy::CurrentPositionJudge()
{
	float sub;
	returnForce = VGet(10.0f, 0.0f, 0.0f);
	returnForce.y = JUMP_DIRECTION_Y;

	//エネミーの現在地が右端寄りなら
	if (SCREEN_RIGHTMOST - position.x <= SCREEN_CENTER)
	{
		
		returnForce = VScale(returnForce, -0.5f);
		//画面端に戻る
		attackType = BACK;
		currentRightPosition = true;		//右端にいる状態に

		//左端に近くなったら
		if (position.x - SCREEN_LEFTMOST <= 150.0f)
		{
			
		}
	}
	//エネミーの現在地が左寄りなら
	else
	{
		returnForce = VScale(returnForce, 0.5f);
		//画面端に戻る
		attackType = BACK;
		currentRightPosition = false;		//左端にいる状態に

		//右端が近くなったら
		if (SCREEN_RIGHTMOST - position.x <= 150.0f)
		{
			
		}
	}
}


void Enemy::UpdateFine()
{
	if (pUpdate != nullptr)
	{
		(this->*pUpdate)();			//各状態の更新処理
	}
}

void Enemy::UpdateCaution()
{

}

void Enemy::UpdateDenger()
{

}

void Enemy::UpdateNormal()
{
	stopTime += DeltaTime::GetInstace().GetDeltaTime();
	if (stopTime > 1.5f)
	{
		stopMove = false;
	}
	InputAction();
	Move();

}

void Enemy::UpdateAttack()
{
	switch (attackType)
	{
	case ASSAULT:
		Assault();
		break;
	case BULLET:
		break;
	case JUDGE:
		CurrentPositionJudge();
		break;
	case BACK:
		Back();
		break;
	}
	
}

void Enemy::UpdateSlide()
{
	Slide();
}

void Enemy::UpdateDamage()
{

}

void Enemy::InputAction()
{
	inputDirection = ZERO_VECTOR;
	inputDirection += vec * speed;
	
	if ( position.x >= SCREEN_RIGHTMOST)
	{
		vec = LEFT;
	}
	else if(position.x <= SCREEN_LEFTMOST)
	{
		vec = RIGHT;
	}

	collisionSphere.Move(position);
}
