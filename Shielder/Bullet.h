#pragma once

class Bullet final : public Mover
{
public:
	enum State
	{
		NONE,			//存在しない
		NORMAL,			//飛んでいる（通常）
		SLOW			//飛んでいる（ゆっくり）
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

	void OnHitBreak();			//何かに当たって壊れる

private:
	//定数
	static const float SPEED;							  //移動速度
	static const float SCALE_BY_DIRECTION_FOR_CORRECTION; // 位置補正用に向きベクトルに掛ける倍率

	State state;		//現在の状態
	VECTOR velocity;	//現在の速度、加速度、向きを持つ、マイナスにもなる
	float speed;		//現在の速度

	My3dLib::Sphere collisionSphere;			//当たり判定用球

	void Move();		//移動処理
};

