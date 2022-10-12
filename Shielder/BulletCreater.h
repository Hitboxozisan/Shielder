#pragma once

class Bullet;

class BulletCreater
{
public:
	BulletCreater();
	~BulletCreater();

	Bullet* Create(const VECTOR& inPosition, const VECTOR& inDirection);
	bool isCreatableCheck();

private:
	//BulletCreater();
	BulletCreater(const BulletCreater&);
	void operator=(const BulletCreater&);

	//���X�g�ւ̃|�C���^
	std::list<Bullet*>* const activeBullet;
	std::list<Bullet*>* const deActiveBullet;
};

