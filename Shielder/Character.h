#pragma once

#include "Mover.h"

using namespace My3dLib;

class Shield;

//���
//namespace CharacterState
//{
//	enum State
//	{
//		NONE,		//���݂��Ȃ�
//		NORMAL,		//�ʏ�
//		ATTACK,		//�U��
//		DEFENSE,	//�h��
//		SLIDE,		//�h�䎞�ɐڐG
//		DAMAGE,		//��e
//		DEAD		//���S
//	};
//}

class Character : public Mover
{
public:
	Character();
	virtual ~Character();

	virtual void Initialize();
	void Finalize();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void OnHitOtherCharacter(const VECTOR& forceDirection) = 0;		//���̃L�����N�^�[�Ɠ�������
	virtual void OnHitShield(const VECTOR& adjust) = 0;				//���ƃL�����N�^�[����������
	void MoveFinish();				//�ړ��\��n�Ɏ��ۂɈړ�����


	const float& GetHitPoint() const { return hitPoint; }							//���݂�hitPoint��Ԃ�
	const VECTOR& GetNextPosition() const  { return nextPosition; }					//�ړ��\��ʒu��Ԃ�
	const VECTOR GetCollisionShere() const { return collisionSphere.worldCenter; }	//�����蔻�苅��Ԃ�
	const float GetCollisionRadius() const { return collisionSphere.radius; }		//�����蔻�苅�̔��a��Ԃ�
	const bool IsCollidableState() const;											//�����ƐڐG�ł����Ԃ�
	const float GetInvicibleTime() const { return invincibleTime; }					//���G���Ԃ̌o�ߎ��Ԃ�Ԃ�

	Shield* GetShieldPointer() const { return shield; }

	//�f�o�b�O�p
	const VECTOR& GetVelocity() const { return velocity; }		// ���݂̑��x�A�����x�A����
	const float& GetTrunkPoint() const { return trunkPoint; }

protected:
	VECTOR inputDirection;

	int   id;				// �L�����̎��ʔԍ�
	float hitPoint;			// �L�����̗̑�
	float trunkPoint;		// �̊��Q�[�W�̗�
	float speed;			// �ړ����x
	VECTOR velocity;		// ���݂̑��x�A�����x�A����
	VECTOR nextPosition;	// �ړ��\��̃|�W�V����
	VECTOR nextDirction;	// �ړ���̗\��̌���
	VECTOR prevPosition;	// �O�t���[���̃|�W�V����
	VECTOR prevDirection;	// �O�t���[���̌���
	bool   noDrawFrame;		// �`�悵�Ȃ��t���[����
	float invincibleTime;	// ���G����

	//CharacterState::State state;	

	Shield* shield;			//���̃C���X�^���X
	Sphere collisionSphere;	//�����蔻�苅
private:
	//�萔
	/*static const float COLLIDE_RADIUS;
	static const float NORMAL_SPEED;
	static const float DEFENSE_SPEED;*/
	
	

	virtual void InputAction() = 0;
};

