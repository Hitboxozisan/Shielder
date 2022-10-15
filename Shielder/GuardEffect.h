#pragma once
class GuardEffect
{
public:
	
	void Initialize();
	void Activate(VECTOR inPosition);
	void Deactivate();

	void PlayEffect();
private:
	GuardEffect();
	GuardEffect(const GuardEffect&);
	void operator=(const GuardEffect&);
	~GuardEffect();

	int effectHandle;		//�G�t�F�N�g�n���h��
	VECTOR position;		

	int playEffectHandle;
};

