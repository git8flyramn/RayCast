//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────

//  Sample3D.hlsl (頂点シェーダ)
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//Texture2D g_OutLineTexture : register(s1)
//
Texture2D g_ToonTexture : register(s1);
SamplerState g_Toonsampler : register(t1);
//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    row_major float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    row_major float4x4 matWolrd; //ワールド行列
    row_major float4x4 matNomal; //法線変換行列
    float4 diffuseColor;
    float4 diffusefactor;
    float4 specular; //スペキュラ
    float4 shininess;
    float4 ambient;
    bool useTexture; //テクスチャを使うかどうか
    
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
                 //セマンティクス
    float4 wpos : Position0; //ワールド座標
    float4 spos : SV_POSITION; //スクリーン座標
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色(明るさ)
    float4 normal : NORMAL; //法線ベクトル
    float4 eyev : POSITION1; //視線ポジション
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL) : SV_POSITION
{
    
    float4 outPos;
    normal.w = 0;
    pos = pos + normal * 0.1;
    pos = mul(pos, matWVP);
    return pos;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(float4 pos : SV_POSITION) : SV_Target
{
    return float4(0.0, 0.0, 0.0, 1.0);
}