#include "Model.h"
#include "Direct3D.h"
namespace Model
{
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	ModelData* pModelData = new ModelData;
	pModelData->filename_ = filename;
	pModelData->pfbx_ = nullptr;

	for (auto& itr : modelList)
	{
		if (itr->filename_ == filename)
		{
			pModelData->pfbx_ = itr->pfbx_;
			break;
	    }
	}
	if (pModelData->pfbx_ == nullptr)
	{
		pModelData->pfbx_ = new Fbx;
		pModelData->pfbx_->Load(filename.c_str());
	}
	modelList.push_back(pModelData);
	return((int)(modelList.size() - 1));

}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->pfbx_->Draw(modelList[hModel]->transform_);

}

void Model::DrawPseudoNormal(int hModel)
{
	modelList[hModel]->pfbx_->DrawPseudoNormal(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false;//参照されているか
	for (int i = 0; i < modelList.size(); i++)
	{
		isReffered = false;
		for (int j = i + 1; j < modelList.size(); j++)
		{
			//modelListの中にあるモデルの消去
			if (modelList[i]->pfbx_ == modelList[j]->pfbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->pfbx_);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear(); //配列の中身を空にする(念のために)
}

void Model::Raycast(int hModel, RayCastData& rayData)
{
	modelList[hModel]->transform_.Calculation();

    //ワールド行列取得
	XMMATRIX worldMatrix = modelList[hModel]->transform_.GetWorldMatrix();
   
	//ワールド行列の逆行列
	XMMATRIX wInv = XMMatrixInverse(nullptr,worldMatrix);

	//レイの通過点を求める(ワールド空間のレイの始点からdir方向に進む直線上の点を計算)
	XMVECTOR vDirVec{rayData.start.x + rayData.dir.x,
				    rayData.start.y + rayData.dir.y,
					rayData.start.z + rayData.dir.z,0.0f };


	XMVECTOR vstart = XMLoadFloat4(&rayData.start);

	//vstartをワールド逆行列に変換
	vstart = XMVector3Transform(vstart, wInv);

	XMStoreFloat4(&rayData.start, vstart);
	
	vDirVec = XMVector3Transform(vDirVec, wInv);
	
	//rayData.dirからrayData.start - (始点から方向ベクトルをちょい伸ばした先)に向かうベクトルにする
	XMVECTOR dirAtLocal = XMVectorSubtract(vDirVec,vstart);
	dirAtLocal = XMVector4Normalize(dirAtLocal);
	XMStoreFloat4(&rayData.dir, dirAtLocal);

	//指定したモデル番号のFBXにレイキャスト
	modelList[hModel]->pfbx_->RayCast(rayData);
}
