#pragma once
#include "Engine//GameObject.h"
class Texture;
class TestScene :
    public GameObject
{
public:
	TestScene(GameObject* parent);
	~TestScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	Texture* tex;
};

