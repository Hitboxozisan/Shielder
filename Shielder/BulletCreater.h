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

	//リストへのポインタ
	std::list<Bullet*>* const activeBullet;
	std::list<Bullet*>* const deActiveBullet;
};

