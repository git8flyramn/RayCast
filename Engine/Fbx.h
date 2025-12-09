#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Texture.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

namespace Math
{
	//行列式を解く関数
	float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c);
	//Rayと三角形との当たり判定
	bool Intersect(XMFLOAT3 origin, XMFLOAT3 ray, XMFLOAT3  v1, XMFLOAT3 v2, float& dist);
}
//RayCastのためのデータを用意
struct RayCastData
{
	XMFLOAT4 start;//Rayの始点
	XMFLOAT4 dir; //Rayの方向
	bool isHit;  //当たったかどうか
	float dist; //始点からの距離

};

class Fbx
{
public:
	Fbx();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();

	void InitVertex(FbxMesh* mesh);
	void InitIndex(FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(FbxNode* pNode);

private:
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuse;
		BOOL		materialFlag; //マテリアルがあるかないか
	};
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//バッファの皆さん
	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	std::vector<MATERIAL> pMaterialList_;
	std::vector<int> indexCount_;//マテリアルごとのインデックス数

	int vertexCount_;
	int polygonCount_;
	int materialCount_;

	//前のデータの復活
	std::vector<VERTEX> pVertices_;
	std::vector<std::vector<int>> ppIndex_; //マテリアルごとのインデックスデータ[material][index]
	//auto& arr = ppIndex_[1];
	//arr[0]からarr[index - 1]までになる
    void RayCast(RayCastData& rayData);

};