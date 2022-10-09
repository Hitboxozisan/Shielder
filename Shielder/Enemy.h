#pragma once

#include "Character.h"

class Enemy final : public Character
{
public:
	//残り体力
	enum EnemyState
	{
		FINE,
		CAUTION,
		DENGER
	};

	//行動状態
	enum State
	{
		NONE,		//存在しない
		NORMAL,		//通常
		ATTACK,		//攻撃
		SLIDE,		//跳ね返る
		DAMAGE,		//被弾
		DEAD		//死亡
	};

	Enemy();
	~Enemy();
	void Initialize();
	void Update();

	void Draw();
	void OnHitOtherCharacter(const VECTOR& forceDirection);		//他のキャラクターと当たった
	void OnHitShield(const VECTOR& adjust);				//盾とキャラクターが当たった

	//const bool IsCollidableState() const;

private:
	static const float COLLIDE_RADIUS;
	static const float NORMAL_SPEED;
	static const float DEFENSE_SPEED;

	float stopTime;					//停止してからの経過時間
	bool stopMove;					//停止しているか


	void (Enemy::*pUpdate)();				//Update関数ポインタ

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

