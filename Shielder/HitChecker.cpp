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
		
		//‚‚Æ“G‚ªÚG‚µ‚½‚ç
		if (HitCheck_Sphere_Sphere(shield->GetPosition(), shield->GetCollisionRadius(),
			character[1]->GetPosition(), character[1]->GetCollisionRadius()) &&
			character[0]->IsCollidableState())
		{
			//printfDx("HitShield");
			sub = VScale(sub, 1.0f);				//‚«”ò‚Î‚·•ûŒü‚Í‹t•ûŒü
			VECTOR forceDirection = VNorm(sub);
			character[0]->OnHitShield(forceDirection);
			character[1]->OnHitShield(forceDirection);
			
		}
	}
}

void HitChecker::CharacterAndCharacter(Character** character)
{
	//ƒvƒŒƒCƒ„[‚ªÚG‰Â”\‚È‚ç
	//if (character[0]->IsCollidableState() == false)
	//{
	//	return;
	//}
	////ƒGƒlƒ~[‚ªÚG‰Â”\‚È‚ç
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
		sub = VScale(sub, 1.0f);		//‚«”ò‚Î‚·•ûŒü‚Í‹t•ûŒü
		VECTOR forceDirection = VNorm(sub);
		character[0]->OnHitOtherCharacter(forceDirection);
		character[1]->OnHitOtherCharacter(forceDirection);
		
		//’µ‚Ë•Ô‚³‚¹‚é
		//character[0]->OnHitOtherCharacter();
	}
}
