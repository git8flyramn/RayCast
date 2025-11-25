#pragma once
#include "Engine//GameObject.h"
#include "Engine//Fbx.h"

class Player;
class Bullet : public GameObject 
{

public:
	Bullet(GameObject* parent);
	~Bullet();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release()override;
private:
	Player* player;
	Fbx* pFbx;
	bool isShot;
	int life;
};

