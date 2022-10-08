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
	//�ǂݍ��ݎ��s�ŃG���[
	if (modelHandle < 0)
	{
		printfDx("���f���f�[�^�̓ǂݍ���_Player\n");
	}

	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));

	//�����蔻�苅��ݒ�
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

	//�����蔻��f�o�b�N�`��
	DrawSphere3D(collisionSphere.worldCenter, collisionSphere.radius,
		8, GetColor(0, 255, 0), 0, FALSE);
}

void Enemy::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	//�ړ����~�߂�
	stopMove = true;
}

void Enemy::OnHitShield(const VECTOR& adjust)
{
	//�O���velocity�����Z�b�g����
	velocity = ZERO_VECTOR;

	VECTOR force = adjust;
	force.y = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���
	force.z = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���

	//�W���X�g�K�[�h����Ȃ��Ȃ�傫����ނ�����
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

	//�ʒu��ݒ�
	MV1SetPosition(modelHandle, position);

	//�ړ��L�[�������Ă��Ȃ��ꍇ�͌������Œ肷��
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
