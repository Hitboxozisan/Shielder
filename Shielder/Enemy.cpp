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
	//�ǂݍ��ݎ��s�ŃG���[
	if (modelHandle < 0)
	{
		printfDx("���f���f�[�^�̓ǂݍ���_Enemy\n");
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
	DrawFormatString(300, 130, GetColor(255, 255, 255), "E : %f", this->GetTrunk(), TRUE);		//�̊��Q�[�W�\��
	DrawFormatString(300, 170, GetColor(255, 255, 255), "MD : %f", movedDistance, TRUE);

	//�����蔻��f�o�b�N�`��
	DrawSphere3D(collisionSphere.worldCenter, collisionSphere.radius,
		8, GetColor(0, 255, 0), 0, FALSE);
}

void Enemy::OnHitOtherCharacter(const VECTOR& forceDirection)
{
	stopTime = 0.0f;
	//�ړ����~�߂�
	stopMove = true;
}

void Enemy::OnHitShield(const VECTOR& adjust)
{

	//�O���velocity�����Z�b�g����
	velocity = ZERO_VECTOR;
	stopTime = 0.0f;
	stopMove = true;

	VECTOR force = adjust;
	force.y = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���
	force.z = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���

	force = VScale(force, -3.0f);

	//��ނ�����
	velocity = VAdd(velocity, force);
	

	trunkPoint -= DECREMENT_TRUNK_POINT;		//�̊��Q�[�W����
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

	//�ʒu��ݒ�
	MV1SetPosition(modelHandle, position);

	//�ړ��L�[�������Ă��Ȃ��ꍇ�͌������Œ肷��
	if (!VSquareSize(inputDirection) == 0.0f)
	{
		MV1SetRotationYUseDir(modelHandle, direction, 0.0f);
	}

}

/// <summary>
/// �ːi�U��
/// </summary>
void Enemy::Assault()
{
	
	VECTOR force = VGet(1.0f, 0.0f, 0.0f);			//�ړ��ʂ�ݒ�
	force.y = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���
	force.z = 0.0f;			//�ςȕ����ɓ����Ȃ��悤�ɂ���

	//�G�l�~�[�̌��ݒn���E�[���Ȃ�
	if (currentRightPosition)
	{
		force = VScale(force, 0.5f);		//�����ɗ͂�^����
	}
	else
	{
		force = VScale(force, -0.5f);		//�E���ɗ͂�^����
	}
	
	//�ːi������
	if (movedDistance <= 300.0f && movedDistance >= -300.0f)
	{
		stopTime = 0.0f;
		velocity = VAdd(velocity, force);
		nextPosition = VAdd(nextPosition, velocity);
	}
	
	//��~���Ԃ𒴂�����ːi�ĊJ
	if (stopTime >= 1.0f)
	{
		++assaultCount;
		movedDistance = 0.0f;
		force = VGet(1.0f, 0.0f, 0.0f);
	}

	//3��ːi�����玟�̍s���Ɉڂ�
	if (assaultCount >= 3.0f)
	{
		attackType = JUDGE;
	}

	stopTime += DeltaTime::GetInstace().GetDeltaTime();
	movedDistance += velocity.x;			//�i�񂾋��������Z
}

void Enemy::Back()
{
	//�����p�ϐ�
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<int> next(0, ATTACK_AMOUST);

	int attackAmoust = ATTACK_AMOUST;
	returnForce.y -= GRAVITY;

	//��ʒ[�܂ňړ�
	if (position.x >= SCREEN_LEFTMOST && position.x <= SCREEN_RIGHTMOST)
	{
	}
	else
	{
		returnForce.x = 0.0f;		//���ړ����~������
	}

	nextPosition = VAdd(nextPosition, returnForce);			//��ʒ[�܂ňړ�

	//�n�ʂɒ��n�����痎�����~�߂�
	if (nextPosition.y <= 0.0f)
	{
		velocity = ZERO_VECTOR;
		assaultCount = 0.0f;		//�ːi�񐔂����Z�b�g
		returnForce = ZERO_VECTOR;
		position.y = 0.0f;
		nextPosition.y = 0.0f;
		int nextAttack = next(eng);	//���̍s�����w��(������)
		attackType = ASSAULT;		//���̏�ԂɈڍs����
	}
}

void Enemy::Slide()
{
	//���点��
	if (velocity.x <= 0.0f)
	{
		velocity.x += FRICTION_FORCE;
	}
	else
	{
		velocity.x -= FRICTION_FORCE;
	}

	//�~�܂������ʒ[��Ԃɖ߂�
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

	//�G�l�~�[�̌��ݒn���E�[���Ȃ�
	if (SCREEN_RIGHTMOST - position.x <= SCREEN_CENTER)
	{
		
		returnForce = VScale(returnForce, -0.5f);
		//��ʒ[�ɖ߂�
		attackType = BACK;
		currentRightPosition = true;		//�E�[�ɂ����Ԃ�

		//���[�ɋ߂��Ȃ�����
		if (position.x - SCREEN_LEFTMOST <= 150.0f)
		{
			
		}
	}
	//�G�l�~�[�̌��ݒn�������Ȃ�
	else
	{
		returnForce = VScale(returnForce, 0.5f);
		//��ʒ[�ɖ߂�
		attackType = BACK;
		currentRightPosition = false;		//���[�ɂ����Ԃ�

		//�E�[���߂��Ȃ�����
		if (SCREEN_RIGHTMOST - position.x <= 150.0f)
		{
			
		}
	}
}


void Enemy::UpdateFine()
{
	if (pUpdate != nullptr)
	{
		(this->*pUpdate)();			//�e��Ԃ̍X�V����
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
