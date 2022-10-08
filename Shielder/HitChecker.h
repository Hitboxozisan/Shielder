#pragma once

class Character;
class Player;
class Enemy;
class Shield;

class HitChecker final
{
public:
	HitChecker(int inCharacaterAmount);
	~HitChecker();

	void Check(Character **character, Shield *shield);

private:

	void ShieldAndCharacter(Character** character, Shield *shield);
	void CharacterAndCharacter(Character** character);

	int characterAmount;
};

