#include "Pch.h"
#include "HitChecker.h"

#include "GameDefine.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Shield.h"

HitChecker::HitChecker(int inCharacaterAmount)
	:characterAmount(inCharacaterAmount)
{
}

HitChecker::~HitChecker()
{
}

void HitChecker::Check(Character** character, Shield *shield)
{
	CharacterAndCharacter(character);
	ShieldAndCharacter(character, shield);

	character[0]->MoveFinish();
	character[1]->MoveFinish();
}

void HitChecker::ShieldAndCharacter(Character** character, Shield *shield)
{
	if (shield != nullptr)
	{
		VECTOR characterPos = character[0]->GetPosition();
		characterPos.y = 0.0f;

		VECTOR characterOtherPos = character[1]->GetPosition();
		characterOtherPos.y = 0.0f;

		VECTOR sub = VSub(characterPos, characterOtherPos);
		//sub.y = 0.0f;

		float length = VSize(sub);
		
		//���ƓG���ڐG������
		if (HitCheck_Sphere_Sphere(shield->GetPosition(), shield->GetCollisionRadius(),
			character[1]->GetPosition(), character[1]->GetCollisionRadius()) &&
			character[0]->IsCollidableState())
		{
			//printfDx("HitShield");
			sub = VScale(sub, 1.0f);				//������΂������͋t����
			VECTOR forceDirection = VNorm(sub);
			character[0]->OnHitShield(forceDirection);
			character[1]->OnHitShield(forceDirection);
			
		}
	}
}

void HitChecker::CharacterAndCharacter(Character** character)
{
	//�v���C���[���ڐG�\�Ȃ�
	//if (character[0]->IsCollidableState() == false)
	//{
	//	return;
	//}
	////�G�l�~�[���ڐG�\�Ȃ�
	//if (character[1]->IsCollidableState() == false)
	//{
	//	return;
	//}

	VECTOR characterPos = character[0]->GetPosition();
	characterPos.y = 0.0f;

	VECTOR characterOtherPos = character[1]->GetPosition();
	characterOtherPos.y = 0.0f;

	VECTOR sub = VSub(characterPos, characterOtherPos);
	//sub.y = 0.0f;

	float length = VSize(sub);

	if (HitCheck_Sphere_Sphere(character[0]->GetPosition(), character[0]->GetCollisionRadius(),
		character[1]->GetPosition(), character[1]->GetCollisionRadius()) &&
		character[0]->IsCollidableState())
	{
		//printfDx("Hit");
		sub = VScale(sub, 1.0f);		//������΂������͋t����
		VECTOR forceDirection = VNorm(sub);
		character[0]->OnHitOtherCharacter(forceDirection);
		character[1]->OnHitOtherCharacter(forceDirection);
		
		//���˕Ԃ�����
		//character[0]->OnHitOtherCharacter();
	}
}
