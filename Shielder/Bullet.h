#pragma once

class Bullet final : public Mover
{
public:
	enum State
	{
		NONE,			//���݂��Ȃ�
		NORMAL,			//���ł���i�ʏ�j
		SLOW			//���ł���i�������j
	};

	Bullet();
	~Bullet();

	void Initialize();
	void Finalize();
	void Activate(const VECTOR& inPosition, const VECTOR& inDirection);
	void Deactivate();
	bool Update();
	void Draw();

	void SetToFrontOfEnemy(const VECTOR& inPosition, const VECTOR& inDirection);

	State GetState() const;
	bool IsCollidableState() const;

	void OnHitBreak();			//�����ɓ������ĉ���

private:
	//�萔
	static const float SPEED;							  //�ړ����x
	static const float SCALE_BY_DIRECTION_FOR_CORRECTION; // �ʒu�␳�p�Ɍ����x�N�g���Ɋ|����{��

	State state;		//���݂̏��
	VECTOR velocity;	//���݂̑��x�A�����x�A���������A�}�C�i�X�ɂ��Ȃ�
	float speed;		//���݂̑��x

	My3dLib::Sphere collisionSphere;			//�����蔻��p��

	void Move();		//�ړ�����
};

