#include "Pch.h"

#include "Character.h"
#include "ModelManager.h"
#include "Shield.h"
#include "Bullet.h"
#include "BulletCreater.h"

using namespace My3dLib;

//const float Character::HIT_OTHER_CHARACTER_DIRECTION_Y = 5.0f;
//const float Character::GRAVITY = 0.1f;

Character::Character()
	:speed(0.0f)
	,velocity()
	,nextPosition()
	,nextDirction()
	,prevPosition()
	,noDrawFrame()
	,bulletCreater()
{
}

Character::~Character()
{
	if (modelHandle != NULL)
	{
		Finalize();
	}
}

void Character::Initialize()
{
	
}

void Character::Finalize()
{
	MV1DeleteModel(modelHandle);
	modelHandle = NULL;
}

void Character::Update()
{

}

void Character::Draw()
{
	
}

void Character::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	//VECTOR force = forceDirection;
	//force.y = HIT_OTHER_CHARACTER_DIRECTION_Y;
	//
	//force = VScale(force, 5.0f);
	//
	//velocity = VAdd(velocity, force);
	//state = DAMAGE;

	////軽く飛ばす
	////velocity = VAdd(velocity, force);

	////nextPosition = VAdd(nextPosition, forceDirection);
}

void Character::MoveFinish()
{
	/*prevPosition = position;

	position = nextPosition;
	direction = nextPosition;

	MV1SetPosition(modelHandle, position);
	MV1SetRotationYUseDir(modelHandle, direction, 0.0f);*/

	prevPosition = position;
	prevDirection = direction;
	position = nextPosition;
	direction = nextDirction;

	//位置を設定
	MV1SetPosition(modelHandle, position);
	//当たり判定球移動
	collisionSphere.Move(position);

	//移動キーを押していない場合は向きを固定する
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		MV1SetRotationYUseDir(modelHandle, direction, 0.0f);
	}
}

const bool Character::IsCollidableState() const
{
	//無敵時間を過ぎたら無敵状態を解除する
	if (invincibleTime >= 1.0f)
	{
		return true;
	}

	return false;
}


