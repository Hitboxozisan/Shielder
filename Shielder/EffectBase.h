#pragma once
class EffectBase
{
public:
	EffectBase();
	virtual ~EffectBase();

	void Initialize();
	void Finalize();
	void Activate();
	void Deactivate();
	void Update();
	void Draw();


protected:
	VECTOR position;
	
private:
	EffectBase(const EffectBase&);
	void operator=(const EffectBase&);

};

