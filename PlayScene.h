#pragma once
#include "Engine/GameObject.h"
#include "Engine//Fbx.h"
class PlayScene : public GameObject
{
public:
	PlayScene(GameObject* parent);
	~PlayScene();
	void Initialize() override;
	void Draw() override;
	void Update() override;
	void Release() override;

};
