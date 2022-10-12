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
		printfDx("���f���f�[�^�ǂݍ��݂Ɏ��s_Bullet\n");
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

	SetToFrontOfEnemy(inPosition, inDirection);		//�G�l�~�[�̑O���Ɉʒu����

	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));	//���f���g��k��
}

void Bullet::Deactivate()
{
	state = NONE;
}

bool Bullet::Update()
{
	//���݂��Ȃ��Ȃ珈�����Ȃ�
	if (state == NONE)
	{
		return false;
	}

	speed = SPEED;

	//���ł���Ƃ��̏���
	if (state == NORMAL ||
		state == SLOW)
	{
		Move();
	}

	return false;
}

void Bullet::Draw()
{
	//���݂��Ȃ��Ȃ�`�悵�Ȃ�
	if (state == NONE)
	{
		return;
	}

	MV1SetPosition(modelHandle, position);		//3D���f���̃|�W�V�����ݒ�

	MV1DrawModel(modelHandle);	//3D���f���̕`��
}

void Bullet::SetToFrontOfEnemy(const VECTOR& inPosition, const VECTOR& inDirection)
{
	position = inPosition;
	direction = inDirection;

	//�G�l�~�[�̑O���Ɉʒu����
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
	position = VAdd(position, VScale(direction, speed));	//�����Ă�������Ɉړ�
}
