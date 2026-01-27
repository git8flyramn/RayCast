//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────

//  Sample3D.hlsl (頂点シェーダ)
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matWolrd; //ワールド行列
    float4x4 matNomal;
    float4 diffuseColor;
    float4 diffusefactor;
    float4 specular; //スペキュラ
    float4 shininess;
    float4 ambient;
    bool useTexture; //テクスチャを使うかどうか
    
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
                 //セマンティクス
    float4 wpos : SV_Position; //ワールド座標
    float4 spos : SP_POSITION; //スクリーン座標
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色(明るさ)
    float4 normal : NORMAL; //法線ベクトル
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

    //ローカル座標に、ワールド・ビュー・プロジェクション行列を掛けて
    //スクリーン座標に変換し、ピクセルシェーダーへ
    outData.spos = mul(pos, matWVP);
	//ワールド座標も変換し、ピクセルシェーダーへ
    outData.wpos = mul(pos, matWolrd);
    outData.normal = mul(normal, matNomal);
    uv.w = 1;
    outData.uv = uv.xy;
    
    normal = mul(normal, matNomal); //法線ベクトルをワールドビュープロジェクション行列で変換
    normal = normalize(normal); //法線ベクトルの長さを正規化->1にする
    normal.w = 0;
    //float4 light = float4(-1, 0.5, -0.7, 0);
    
    //light = normalize(light);
    //light.w = 0;
    
   // outData.color = clamp(dot(normal, light), 0, 1);
    
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 light = float4(-1, 0.5, -0.7, 0);
    float4 color;
    float3 L;
    float3 N;
    float i;
    L = normalize(light.xyz - inData.wpos.xyz);
    N = normalize(inData.normal.xyz);
    i = saturate(dot(L, N));
    
    if (useTexture == 1)
    {
        color = g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
        color = diffuseColor;
    }
    return color * inData.color;
}