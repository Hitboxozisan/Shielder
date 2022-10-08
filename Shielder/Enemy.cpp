#include "Pch.h"
#include "Enemy.h"

#include "ModelManager.h"

const float Enemy::COLLIDE_RADIUS = 50.0f;
const float Enemy::NORMAL_SPEED = 3.0f;
const float Enemy::DEFENSE_SPEED = 2.0f;

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	vec = LEFT;
	speed = NORMAL_SPEED;
	position = VGet(2400.0f, 1.0f, 100.0f);
	nextPosition = position;
	direction = ZERO_VECTOR;
	nextDirction = direction;

	physical = FINE;
	state = NORMAL;

	modelHandle = MV1DuplicateModel(ModelManager::GetInstance().GetModelHandle(ModelManager::ENEMY));
	//読み込み失敗でエラー
	if (modelHandle < 0)
	{
		printfDx("モデルデータの読み込み_Player\n");
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

	if (!stopMove)
	{
		Move();

		InputAction();
		MoveFinish();
	}
	
}

void Enemy::Draw()
{
	MV1DrawModel(modelHandle);

	//当たり判定デバック描画
	DrawSphere3D(collisionSphere.worldCenter, collisionSphere.radius,
		8, GetColor(0, 255, 0), 0, FALSE);
}

void Enemy::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	//移動を止める
	stopMove = true;
}

void Enemy::OnHitShield(const VECTOR& adjust)
{
	//前回のvelocityをリセットする
	velocity = ZERO_VECTOR;

	VECTOR force = adjust;
	force.y = 0.0f;			//変な方向に動かないようにする
	force.z = 0.0f;			//変な方向に動かないようにする

	//ジャストガードじゃないなら大きく後退させる
	force = VScale(force, 8.0f);

	velocity = VAdd(velocity, force);
	
	//nextPosition = VAdd(nextPosition, adjust);
}

bool Enemy::IsCollidableState() const
{
	return false;
}

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

void Enemy::UpdateFine()
{

}

void Enemy::UpdateCaution()
{
}

void Enemy::UpdateDenger()
{
}

void Enemy::InputAction()
{
	inputDirection = ZERO_VECTOR;
	inputDirection += vec * speed;
	if ( position.x >= 620.0f)
	{
		vec = LEFT;
		position.x = 610.0f;
	}
	else if(position.x <= 0.0f)
	{
		vec = RIGHT;
		position.x = 1.0f;
	}

	collisionSphere.Move(position);
}
