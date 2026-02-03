#pragma once
#include "Engine//GameObject.h"
#include <windows.h>
#include "Engine//Sprite.h"

namespace
{
    const int XSIZE{ 15 };
    const int ZSIZE{ 15 };
    const int MODEL_NUM{ 5 };
}
enum BLOCK_TYPE

{
    DEFAULT,
    LENGA,
    GLASS,
    SAND,
    DONUT,
};

struct CONSTANT_BUFFER_STAGE
{
    XMFLOAT4 lightPosition;//光源の位置
    XMFLOAT4 eyePosition;   //カメラの座標
};
// WATER,
struct sData

{

    BLOCK_TYPE type;//ブロックのタイプ

    int height;//ブロックを積む高さ
   
    
};
class Stage : public GameObject
{ 
public:
    /*void SetBlock(BLOCK_TYPE type, int x, int z) { GetT(x, z).type = type; }
    void SetBlockHeight(int x, int z, int height) { GetT(x, z).height = height;}
    sData& GetT(int x, int z) { return sTable[z][x]; }*/
public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Draw() override;
    void Update()override;
    void Release()override;
   // BOOL localProc(HWND hWnd, UINT messsage, WPARAM wParam, LPARAM lParam);
    //操作パネル用プロシージャ
   // BOOL ManuProc(HWND hWnd, UINT messsage, WPARAM wParam, LPARAM lParam);
private:
   
    //sData sTable[ZSIZE][XSIZE];
    // int hModel[MODEL_NUM];
    // int mode_;//0 : 上げる 1:下げる 2:種類変更
    // int select_;//ボックスの種類
    // int model_;
    //Sprite* pImage;

    int hball_;
    int hRoom_;
    int hGround_;
    int hDonut_;
     void InitConstantBuffer();
     ID3D11Buffer* pConstantBuffer_;
};


