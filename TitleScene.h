#pragma once
#include "Engine/GameObject.h"
#include "Engine/Fbx.h"
class TitleScene : GameObject
{
public:
	TitleScene(GameObject* parent);
	~TitleScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	Fbx* fbx;
};
