#include "GameObject.h"
#include "SphereCollider.h"
#include <windows.h>
GameObject::GameObject(): pParent_(nullptr)
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	:pParent_(parent),objectName_(name),isDead_(false)
{
	if (parent != nullptr)
	{
		transform_.pParent_ = &(parent->transform_);
	}
}

GameObject::~GameObject()
{
}

void GameObject::DrawSub()
{
	
	Draw();
	/*for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->DrawSub();
	}*/
	for (auto child : childList_)
	{
		child->DrawSub();
	}
}

void GameObject::UpdateSub()
{
	//本体の機能を呼び出す
	transform_.Calculation();
	this->Update();
	RoundRobin(GetRootJob());
	for (auto child : childList_)
	{
		child->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->isDead_)
		{
			(*itr)->ReleaseSub();
			delete(*itr);
			itr = childList_.erase(itr);
		}
		else
		{
			++itr;
		}
	}

}

void GameObject::ReleaseSub()
{
	this->Release();//自分を解放
	for (auto child : childList_)
	{
		child->ReleaseSub();
	}
	
	
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	transform_.position_ = position;
}

void GameObject::SetPosition(float x, float y, float z)
{
	SetPosition(XMFLOAT3(x, y, z));
}

void GameObject::KillMe()
{
	isDead_ = true;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
	{
		return this;//rootJobだよ
	}
	else
	{
		return pParent_->GetRootJob();
	}
}

GameObject* GameObject::FindChildObject(const string& name)
{
	if (this->objectName_ == name)
	{
		return this;//自分が探されていたオブジェクト
	}
	else
	{
		for (auto child : childList_)
		{
			GameObject* result = child->FindChildObject(name);
			if (result != nullptr)
			{
				return result;
			}
		}
		return nullptr;//自分が探されていたオブジェクト
	}
}

GameObject* GameObject::FindObject(const string& name)
{
	GameObject* rootJob = GetRootJob();
	GameObject* result = rootJob->FindChildObject(name);
	return result;
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pTarget)
{
	//this->pCollier_とpTarget->pCollider_はぶつかってますか？
	//?閾値＝お互いの半径＋半径
	float thisR = this->pCollider_->GetRadius();
	float tgtR = pTarget->pCollider_->GetRadius();
	float thre = (thisR + tgtR) * (thisR + tgtR);
	//2つのコライダーの距離を測る

	XMFLOAT3 thisP = this->transform_.position_;
	XMFLOAT3 tgtP = pTarget->transform_.position_;
	float dist = (thisP.x - tgtP.x) * (thisP.x - tgtP.x) +
		         (thisP.y - tgtP.y) * (thisP.y - tgtP.y) +
		         (thisP.z - tgtP.z) * (thisP.z - tgtP.z);
	if (dist <= thre)
	{

		MessageBoxA(0, "ぶつかった", "Collider", MB_OK);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	//1 自分にコライダーが無かったらスルー
	if (pCollider_ == nullptr)
	{
		return;
	}
   ////2 自分とターゲット自体のコライダーの当たり判定
	if (pTarget->pCollider_ != nullptr && pTarget->pCollider_ != pCollider_)
	{
		Collision(pTarget);
	}
	//再帰的なやつで、ターゲットの子オブジェクトを当たり判定をしていく
	for (auto itr : pTarget->childList_)
	{
		RoundRobin(itr);
	}
}

void GameObject::OnCollisiton()
{
}



