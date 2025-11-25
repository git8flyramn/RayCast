#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

//namespace グローバルの変数ぽく使う
using namespace DirectX;
struct CONSTANT_BUFFER
{
	DirectX::XMMATRIX matWorld; //ワールド行列
};

//頂点情報
struct VERTEX
{
	XMFLOAT4 pos;
	XMFLOAT2 uv;
};

class Texture;
class Sprite
{
public:
	Sprite();
	virtual ~Sprite();
	virtual HRESULT Initialize();
	void Draw(DirectX::XMMATRIX& worldMatrix);
	void Release();
protected:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;    //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//定数バッファ
	Texture* pTexture_;

};