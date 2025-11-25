#include "Bullet.h"
#include "Engine//SphereCollider.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Player.h"
Bullet::Bullet(GameObject* parent) 
	:GameObject(parent, "Bullet"), pFbx(nullptr), isShot(false),life(60)
{
	
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	pFbx = new Fbx;
	transform_.scale_.x = 0.5f;
	transform_.scale_.y = 0.5f;
	transform_.scale_.z = 0.5f;
	pFbx->Load("daikon.fbx");
    transform_.position_ = { -1.0f,0.0f,0.0f};
	SphereCollider* col = new SphereCollider(0.5f);
	AddCollider(col);
}

void Bullet::Update()
{
	
	if (Input::IsKey(DIK_SPACE))
	{
		life--;
		transform_.position_.x += 0.2f;
	}
	if (life < 0)
	{
		life = 120;
		transform_.position_.x = 0.0f;
	}
	if (Input::IsKey(DIK_W))
	{
		transform_.position_.y += 0.2f;
	}
	if (Input::IsKey(DIK_S))
	{
		transform_.position_.y -= 0.2f;
	}

}

void Bullet::Draw()
{
	
		pFbx->Draw(transform_);
	
	
}

void Bullet::Release()
{
}


