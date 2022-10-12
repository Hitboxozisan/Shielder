#include "Pch.h"
#include "Mover.h"

#include "Bullet.h"

#include "ModelManager.h"

const float Bullet::SPEED = 5.0f;
const float Bullet::SCALE_BY_DIRECTION_FOR_CORRECTION = 5.0f;

Bullet::Bullet()
	:state(NONE)
	,speed(0.0f)
{
}

Bullet::~Bullet()
{
	if (modelHandle != NULL)
	{
		Finalize();
	}
}

void Bullet::Initialize()
{
	modelHandle = MV1DuplicateModel(ModelManager::GetInstance().GetModelHandle(ModelManager::BULLET));

	if (modelHandle < 0)
	{
		printfDx("モデルデータ読み込みに失敗_Bullet\n");
	}

}

void Bullet::Finalize()
{
	MV1DeleteModel(modelHandle);
	modelHandle = NULL;
}

void Bullet::Activate(const VECTOR& inPosition, const VECTOR& inDirection)
{
	position = inPosition;
	direction = inDirection;
	velocity = ZERO_VECTOR;

	state;

	SetToFrontOfEnemy(inPosition, inDirection);		//エネミーの前方に位置調整

	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));	//モデル拡大縮小
}

void Bullet::Deactivate()
{
	state = NONE;
}

bool Bullet::Update()
{
	//存在しないなら処理しない
	if (state == NONE)
	{
		return false;
	}

	speed = SPEED;

	//飛んでいるときの処理
	if (state == NORMAL ||
		state == SLOW)
	{
		Move();
	}

	return false;
}

void Bullet::Draw()
{
	//存在しないなら描画しない
	if (state == NONE)
	{
		return;
	}

	MV1SetPosition(modelHandle, position);		//3Dモデルのポジション設定

	MV1DrawModel(modelHandle);	//3Dモデルの描画
}

void Bullet::SetToFrontOfEnemy(const VECTOR& inPosition, const VECTOR& inDirection)
{
	position = inPosition;
	direction = inDirection;

	//エネミーの前方に位置調整
	VECTOR distanceToPlayer = VScale(direction, SCALE_BY_DIRECTION_FOR_CORRECTION);

	position = VAdd(position, distanceToPlayer);
}

Bullet::State Bullet::GetState() const
{
	return State();
}

bool Bullet::IsCollidableState() const
{
	if (state == NORMAL ||
		state == SLOW)
	{
		return true;
	}

	return false;
}

void Bullet::OnHitBreak()
{
	state = NONE;

}

void Bullet::Move()
{
	position = VAdd(position, VScale(direction, speed));	//向いている方向に移動
}
