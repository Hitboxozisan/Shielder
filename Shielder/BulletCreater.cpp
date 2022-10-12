#include "Pch.h"
#include <list>
#include "BulletCreater.h"

BulletCreater::BulletCreater()
	:activeBullet()
	,deActiveBullet()
{
}

BulletCreater::~BulletCreater()
{
}

Bullet* BulletCreater::Create(const VECTOR& inPosition, const VECTOR& inDirection)
{
	if (isCreatableCheck() == false)
	{

	}

	return nullptr;
}

bool BulletCreater::isCreatableCheck()
{
	return false;
}
