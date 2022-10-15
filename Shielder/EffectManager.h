#pragma once
class EffectManager final
{
public:
	EffectManager();
	~EffectManager();

	void Initialize();
	void Finalize();
	void Activate();
	void Deactivate();
	void Update();
	void Draw();


private:
	

	EffectManager(const EffectManager&);
	void operator=(const EffectManager&);


};

