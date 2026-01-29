#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include <string>
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
	explicit Sprite(const char* filename);
	explicit Sprite(const std::wstring& filename);
	virtual ~Sprite();
	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;
	//virtual HRESULT Initialize();
	virtual void Draw(DirectX::XMMATRIX& worldMatrix);
	void Release();
	
protected:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;    //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//定数バッファ
	Texture* pTexture_;

	HRESULT InitializeBuffers();
	void LoadTexture(const char* filename);
	void LoadTexture(const std::wstring& filename);

};