#pragma once

#include "Character.h"

class Enemy final : public Character
{
public:
	//�c��̗�
	enum EnemyState
	{
		FINE,
		CAUTION,
		DENGER
	};

	//�s�����
	enum State
	{
		NONE,		//���݂��Ȃ�
		NORMAL,		//�ʏ�
		ATTACK,
		SLIDE,		//���˕Ԃ�
		DAMAGE,		//��e
		DEAD		//���S
	};

	Enemy();
	~Enemy();
	void Initialize();
	void Update();

	void Draw();
	void OnHitOtherCharacter(const VECTOR& forceDirection);		//���̃L�����N�^�[�Ɠ�������
	void OnHitShield(const VECTOR& adjust);				//���ƃL�����N�^�[����������

	bool IsCollidableState() const;

private:
	static const float COLLIDE_RADIUS;
	static const float NORMAL_SPEED;
	static const float DEFENSE_SPEED;

	bool stopMove;

	void (Enemy::*pUpdate)();				//Update�֐��|�C���^

	VECTOR vec;
	EnemyState physical;
	State state;

	void Move();
	void MoveFinish();
	void UpdateFine();
	void UpdateCaution();
	void UpdateDenger();
	void InputAction();
};

